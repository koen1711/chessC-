#include "Board.h"
#include "../Setup/Setup.h"
#include <iostream>

Board::Board() {
    moveMap.reserve(64);
    // Set the size of the board and initialize it with nullptr

    const char* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    Setup setup = Setup();
    setup.addFenToSetup(fen);

    for (const auto& pair : setup.pieces) {
        std::string piece = pair.first;
        const auto& positions = pair.second;
        for (int position : positions) {
            if (piece == "p") {
                whitePiecesBitboard |= (1ULL << position);
                pawnsBitboard |= (1ULL << position);
            } else if (piece == "P") {
                blackPiecesBitboard |= (1ULL << position);
                pawnsBitboard |= (1ULL << position);
            } else if (piece == "r") {
                whitePiecesBitboard |= (1ULL << position);
                rooksBitboard |= (1ULL << position);
            } else if (piece == "R") {
                blackPiecesBitboard |= (1ULL << position);
                rooksBitboard |= (1ULL << position);
            } else if (piece == "n") {
                whitePiecesBitboard |= (1ULL << position);
                knightsBitboard |= (1ULL << position);
            } else if (piece == "N") {
                blackPiecesBitboard |= (1ULL << position);
                knightsBitboard |= (1ULL << position);
            } else if (piece == "b") {
                whitePiecesBitboard |= (1ULL << position);
                bishopsBitboard |= (1ULL << position);
            } else if (piece == "B") {
                blackPiecesBitboard |= (1ULL << position);
                bishopsBitboard |= (1ULL << position);
            } else if (piece == "q") {
                whitePiecesBitboard |= (1ULL << position);
                queensBitboard |= (1ULL << position);
            } else if (piece == "Q") {
                blackPiecesBitboard |= (1ULL << position);
                queensBitboard |= (1ULL << position);
            } else if (piece == "k") {
                whitePiecesBitboard |= (1ULL << position);
                kingsBitboard |= (1ULL << position);
                whiteKing = position;
            } else if (piece == "K") {
                blackPiecesBitboard |= (1ULL << position);
                kingsBitboard |= (1ULL << position);
                blackKing = position;
            }
        }
    }

    this->moveGenerator = new MoveGenerator(this);
    this->gameLoopClass = new GameLoop(this);
    moveGenerator->GenerateMoves();
}

void Board::gameLoop() const {
    gameLoopClass->gameLoop();
    if (inCheck) {
        if (checkmate) {
        }
    }
    this->moveGenerator->GenerateMoves();
}

void Board::makeMove(Move* move) {
    enPassantLeftBitboard = 0ULL;
    enPassantRightBitboard = 0ULL;
    // check if the piece is from the current turn
    if (getPieceColor(move->fromSquare) != turn) {
        return;
    }

    int from = move->fromSquare;
    int to = move->toSquare;

    // Handle castling separately
    if (move->isCastle) {
        std::cout << "Castling" << std::endl;
        // add to piece flags
        int rookFrom, rookTo;
        if (to == 62) { // White king side castle
            rookFrom = 63;
            rookTo = 61;
        } else if (to == 58) { // White queen side castle
            rookFrom = 56;
            rookTo = 59;
        } else if (to == 6) { // Black king side castle
            rookFrom = 7;
            rookTo = 5;
        } else if (to == 2) { // Black queens side castle
            rookFrom = 0;
            rookTo = 3;
        } else {
            // Invalid castle move
            return;
        }

        // Move the rook
        movePiece(rookFrom, rookTo);
    }

    // Handle en passant
    if (move->isEnPassant) {
        std::cout << "En passant" << std::endl;
        int capturedPawnPos;
        if (turn == ChessColor::COLORWHITE) {
            capturedPawnPos = to + 8;
        } else {
            capturedPawnPos = to - 8;
        }
        removePiece(capturedPawnPos);
    } else {
        if (getPieceType(to) != NONE) {
            // Capture the piece at the destination square
            removePiece(to);
        }
    }

    if (move->isPawnDoubleMove) {
        // get the pawns that are next to the pawn that moved
        int leftPawnPos = to + 1;
        int rightPawnPos = to - 1;
        // check if there are pawns and if so set the en passant flag
        if (getPieceType(leftPawnPos) == PieceType::PAWN && getPieceColor(leftPawnPos) != move->movedColor) {
            // check if we are in the right column
            if ((leftPawnPos % 8 != 7 && move->movedColor == ChessColor::COLORWHITE) || (leftPawnPos % 8 != 0 && move->movedColor == ChessColor::COLORBLACK)) {
                // Get if relative for the other pawn if it is left or right
                enPassantLeftBitboard |= (1ULL << leftPawnPos);
            }
        }
        if (getPieceType(rightPawnPos)  == PieceType::PAWN && getPieceColor(rightPawnPos) != move->movedColor) {
            // Get if relative for the other pawn if it is left or right
            if ((rightPawnPos % 8 != 7 && move->movedColor == ChessColor::COLORWHITE) || (rightPawnPos % 8 != 0 && move->movedColor == ChessColor::COLORBLACK)) {
                enPassantRightBitboard |= (1ULL << rightPawnPos);
            }
        }
    }

    // Handle promotion
    if (move->isPromotion) {
        std::cout << "Promotion" << std::endl;
        promotePawn(to);
    }

    movePiece(from, to);
    //pieceToMove->pFlags->MOVED = true;
    //board[to] = pieceToMove;
    //board[from] = nullptr;
    //pieceToMove->moves.clear();
    moveGenerator->GenerateCapturesOnly();
    turn = move->movedColor == ChessColor::COLORWHITE ? ChessColor::COLORBLACK : ChessColor::COLORWHITE;
    moveMap.clear();
}

void Board::undoMove(Move* move) {
    enPassantLeftBitboard = 0ULL;
    enPassantRightBitboard = 0ULL;
    int from = move->fromSquare;
    int to = move->toSquare;

    movePiece(to, from);

    // Move the captured piece back to its original position
    if (move->capturedPiece != NONE && !move->isEnPassant) {
        // Restore the captured piece if there was any
        switch (move->capturedPiece) {
            case PieceType::PAWN:
                pawnsBitboard |= (1ULL << to);
                break;
            case PieceType::KNIGHT:
                knightsBitboard |= (1ULL << to);
                break;
            case PieceType::BISHOP:
                bishopsBitboard |= (1ULL << to);
                break;
            case PieceType::ROOK:
                rooksBitboard |= (1ULL << to);
                break;
            case PieceType::QUEEN:
                queensBitboard |= (1ULL << to);
                break;
            case PieceType::KING:
                kingsBitboard |= (1ULL << to);
                break;
            case PieceType::NONE:
                break;
        }

        if (move->movedColor == ChessColor::COLORBLACK) {
            whitePiecesBitboard |= (1ULL << to);
        } else {
            blackPiecesBitboard |= (1ULL << to);
        }
    }


    // Handle castling separately
    if (move->isCastle) {
        int rookFrom, rookTo = -1;
        if (to == 62) { // White king side castle
            rookFrom = 63;
            rookTo = 61;
        } else if (to == 58) { // White queen side castle
            rookFrom = 56;
            rookTo = 59;
        } else if (to == 6) { // Black king side castle
            rookFrom = 7;
            rookTo = 5;
        } else if (to == 2) { // Black queen side castle
            rookFrom = 0;
            rookTo = 3;
        }

        if (rookTo != -1) {
            movePiece(rookTo, rookFrom);
        }
    }

    // Handle en passant
    if (move->isEnPassant) {
        int capturedPawnPos = -1;
        if (move->movedColor == ChessColor::COLORWHITE) {
            capturedPawnPos = to + 8;
        } else if (move->movedColor == ChessColor::COLORBLACK) {
            capturedPawnPos = to - 8;
        }

        pawnsBitboard |= (1ULL << capturedPawnPos);
        if (move->movedColor == ChessColor::COLORBLACK) {
            whitePiecesBitboard |= (1ULL << capturedPawnPos);
        } else {
            blackPiecesBitboard |= (1ULL << capturedPawnPos);
        }
    }

    // Handle promotion
    if (move->isPromotion) {
        // Replace the promoted piece with the original pawn
        //pieceToMove = new Pawn(pieceToMove->color, move.fromSquare);
        //board[to] = pieceToMove;
        //pawnsBitboard |= (1ULL << move.fromSquare);
    }


    moveMap.clear();
    moveGenerator->GenerateCapturesOnly();
    // Update the turn
    turn = move->movedColor;
    gameLoop();
}

void Board::removePiece(int position) {
    switch (getPieceType(position)) {
        case PieceType::PAWN:
            pawnsBitboard &= ~(1ULL << position);
            break;
        case PieceType::KNIGHT:
            knightsBitboard &= ~(1ULL << position);
            break;
        case PieceType::BISHOP:
            bishopsBitboard &= ~(1ULL << position);
            break;
        case PieceType::ROOK:
            rooksBitboard &= ~(1ULL << position);
            break;
        case PieceType::QUEEN:
            queensBitboard &= ~(1ULL << position);
            break;
        case PieceType::KING:
            kingsBitboard &= ~(1ULL << position);
            break;
        case NONE:
            break;
    }
    if (getPieceColor(position) == ChessColor::COLORWHITE) {
        whitePiecesBitboard &= ~(1ULL << position);
    } else {
        blackPiecesBitboard &= ~(1ULL << position);
    }
}

void Board::movePiece(int from, int to) {
    switch (getPieceType(from)) {
        case PieceType::PAWN:
            pawnsBitboard &= ~(1ULL << from); // Remove the pawn from the pawns bitboard
            pawnsBitboard |= (1ULL << to); // Add the pawn to the pawns bitboard
            break;
        case PieceType::KNIGHT:
            knightsBitboard &= ~(1ULL << from);
            knightsBitboard |= (1ULL << to);
            break;
        case PieceType::BISHOP:
            bishopsBitboard &= ~(1ULL << from);
            bishopsBitboard |= (1ULL << to);
            break;
        case PieceType::ROOK:
            rooksBitboard &= ~(1ULL << from);
            rooksBitboard |= (1ULL << to);
            break;
        case PieceType::QUEEN:
            queensBitboard &= ~(1ULL << from);
            queensBitboard |= (1ULL << to);
            break;
        case PieceType::KING:
            kingsBitboard &= ~(1ULL << from);
            kingsBitboard |= (1ULL << to);
            if (getPieceColor(from) == ChessColor::COLORWHITE) {
                whiteKing = to;
            } else {
                blackKing = to;
            }
            break;
        case NONE:
            break;
    }

    if (getPieceColor(from) == ChessColor::COLORWHITE) {
        whitePiecesBitboard &= ~(1ULL << from);
        whitePiecesBitboard |= (1ULL << to);
    } else {
        blackPiecesBitboard &= ~(1ULL << from);
        blackPiecesBitboard |= (1ULL << to);
    }
}

void Board::promotePawn(int position) {}

PieceType Board::getPieceType(int position) const {

    // Check if the position is not occupied
    if (!((whitePiecesBitboard | blackPiecesBitboard) & (1ULL << position))) {
        return PieceType::NONE;
    }

    if (this->pawnsBitboard & (1ULL << position)) {
        return PieceType::PAWN;
    } else if (this->knightsBitboard & (1ULL << position)) {
        return PieceType::KNIGHT;
    } else if (this->bishopsBitboard & (1ULL << position)) {
        return PieceType::BISHOP;
    } else if (this->rooksBitboard & (1ULL << position)) {
        return PieceType::ROOK;
    } else if (this->queensBitboard & (1ULL << position)) {
        return PieceType::QUEEN;
    } else if (this->kingsBitboard & (1ULL << position)) {
        return PieceType::KING;
    }
    return PieceType::NONE;
}

ChessColor Board::getPieceColor(int position) const {
    if (whitePiecesBitboard & (1ULL << position)) {
        return ChessColor::COLORWHITE;
    } else if (blackPiecesBitboard & (1ULL << position)) {
        return ChessColor::COLORBLACK;
    }
    return ChessColor::COLORNONE;
}

Directions Board::getPinDirection(int position) {

    if (pinnedPiecesBitboard & (1ULL << position)) {
        for (int i = 0; i < Directions::NUMDIRECTIONS; i++) {
            if (pinDirectionBitboards[i] & (1ULL << position)) {
                return (Directions) i;
            }
        }
    }
    return Directions::NODIRECTION;
}