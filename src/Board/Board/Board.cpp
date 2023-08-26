#include "Board.h"
#include "../Setup/Setup.h"
#include <iostream>

Board::Board() {
    // Set the size of the board and initialize it with nullptr
    board.resize(64, nullptr);

    const char* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    Setup setup = Setup();
    setup.addFenToSetup(fen);

    for (const auto& pair : setup.pieces) {
        std::string piece = pair.first;
        const auto& positions = pair.second;
        for (int position : positions) {
            Piece* pieceToPush;
            if (piece == "p") {
                pieceToPush = new Pawn(ChessColor::COLORWHITE, position);
                pieces.push_back(pieceToPush);
                pawns[ChessColor::COLORWHITE].push_back(pieces.size() - 1);
            } else if (piece == "P") {
                pieceToPush = new Pawn(ChessColor::COLORBLACK, position);
                pieces.push_back(pieceToPush);
                pawns[ChessColor::COLORBLACK].push_back(pieces.size() - 1);
            } else if (piece == "r") {
                pieceToPush = new Rook(ChessColor::COLORWHITE, position);
                pieces.push_back(pieceToPush);
                rooks[ChessColor::COLORWHITE].push_back(pieces.size() - 1);
            } else if (piece == "R") {
                pieceToPush = new Rook(ChessColor::COLORBLACK, position);
                pieces.push_back(pieceToPush);
                rooks[ChessColor::COLORBLACK].push_back(pieces.size() - 1);
            } else if (piece == "n") {
                pieceToPush = new Knight(ChessColor::COLORWHITE, position);
                pieces.push_back(pieceToPush);
                knights[ChessColor::COLORWHITE].push_back(pieces.size() - 1);
            } else if (piece == "N") {
                pieceToPush = new Knight(ChessColor::COLORBLACK, position);
                pieces.push_back(pieceToPush);
                knights[ChessColor::COLORBLACK].push_back(pieces.size() - 1);
            } else if (piece == "b") {
                pieceToPush = new Bishop(ChessColor::COLORWHITE, position);
                pieces.push_back(pieceToPush);
                bishops[ChessColor::COLORWHITE].push_back(pieces.size() - 1);
            } else if (piece == "B") {
                pieceToPush = new Bishop(ChessColor::COLORBLACK, position);
                pieces.push_back(pieceToPush);
                bishops[ChessColor::COLORBLACK].push_back(pieces.size() - 1);
            } else if (piece == "q") {
                pieceToPush = new Queen(ChessColor::COLORWHITE, position);
                pieces.push_back(pieceToPush);
                queens[ChessColor::COLORWHITE].push_back(pieces.size() - 1);
            } else if (piece == "Q") {
                pieceToPush = new Queen(ChessColor::COLORBLACK, position);
                pieces.push_back(pieceToPush);
                queens[ChessColor::COLORBLACK].push_back(pieces.size() - 1);
            } else if (piece == "k") {
                pieceToPush = new King(ChessColor::COLORWHITE, position);
                pieces.push_back(pieceToPush);
                kings[ChessColor::COLORWHITE] = pieces.size() - 1;
            } else if (piece == "K") {
                pieceToPush = new King(ChessColor::COLORBLACK, position);
                pieces.push_back(pieceToPush);
                kings[ChessColor::COLORBLACK] = pieces.size() - 1;
            }
            board[position] = pieceToPush;
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

void Board::movePiece(Move* move) {
    // check if the piece is from the current turn
    if (move->movedPiece->color != turn) {
        return;
    }
    int from = move->fromSquare;
    int to = move->toSquare;

    int row = from / 8;
    int col = from % 8;

    Piece* pieceToMove = board[from];
    pieceToMove->pFlags->MOVED = true;

    // Handle castling separately
    if (move->isCastle) {
        std::cout << "Castling" << std::endl;
        // add to piece flags
        pieceToMove->pFlags->CASTLED = true;
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
        board[rookTo] = board[rookFrom];
        board[rookFrom] = nullptr;
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
        board[to] = move->capturedPiece;
        if (board[to] != nullptr) {
            // Capture the piece at the destination square
            removePiece(to);
        }
    }

    if (move->isPawnDoubleMove) {
        // get the pawns that are next to the pawn that moved
        int leftPawnPos = to - 1;
        int rightPawnPos = to + 1;
        // check if there are pawns and if so set the en passant flag
        if (board[leftPawnPos] != nullptr && board[leftPawnPos]->name == PieceType::PAWN && board[leftPawnPos]->color != pieceToMove->color) {
            // check if we are in the right column
            if (leftPawnPos % 8 != 7) {
                // Get if relative for the other pawn if it is left or right
                board[leftPawnPos]->pFlags->ENPASSANTRIGHT = true;
            }
        }
        if (board[rightPawnPos] != nullptr && board[rightPawnPos]->name == PieceType::PAWN && board[rightPawnPos]->color != pieceToMove->color) {
            // Get if relative for the other pawn if it is left or right
            if (rightPawnPos % 8 != 0) {
                board[rightPawnPos]->pFlags->ENPASSANTLEFT = true;
            }
        }
    }

    // Update the piece's position
    pieceToMove->position = to;

    // Handle promotion
    if (move->isPromotion) {
        std::cout << "Promotion" << std::endl;
        promotePawn(pieceToMove);
        pieceToMove = board[to]; // The promoted pawn is replaced with the new piece
    }

    pieceToMove->pFlags->MOVED = true;
    board[to] = pieceToMove;
    board[from] = nullptr;
    pieceToMove->moves.clear();
    moveGenerator->GenerateCapturesOnly();
    turn = (turn == ChessColor::COLORWHITE) ? ChessColor::COLORBLACK : ChessColor::COLORWHITE;
}

void Board::undoMove(const Move &move) {
    int from = move.fromSquare;
    int to = move.toSquare;
    Piece* pieceToMove = board[to];

    // Move the piece back to its original position
    board[from] = pieceToMove;
    board[to] = move.capturedPiece;
    if (move.capturedPiece != nullptr) {
        // Restore the captured piece if there was any

    }

    // Handle castling separately
    if (move.isCastle) {
        int rookFrom, rookTo;
        if (to == 62) { // White kingside castle
            rookFrom = 63;
            rookTo = 61;
        } else if (to == 58) { // White queenside castle
            rookFrom = 56;
            rookTo = 59;
        } else if (to == 6) { // Black kingside castle
            rookFrom = 7;
            rookTo = 5;
        } else if (to == 2) { // Black queenside castle
            rookFrom = 0;
            rookTo = 3;
        } else {
            // Invalid castle move
            return;
        }

        // Move the rook back to its original position
        board[rookFrom] = board[rookTo];
        board[rookTo] = nullptr;
    }

    // Handle en passant
    if (move.isEnPassant) {
        int capturedPawnPos;
        if (turn == ChessColor::COLORWHITE) {
            capturedPawnPos = to + 8;
        } else {
            capturedPawnPos = to - 8;
        }
        board[capturedPawnPos] = move.capturedPiece;
    }

    if (move.isPawnDoubleMove) {
        // Check if the pawn next to the pawn that moved has the en passant flag
        int leftPawnPos = to - 1;
        int rightPawnPos = to + 1;
        if (board[leftPawnPos] != nullptr && board[leftPawnPos]->name == PieceType::PAWN && board[leftPawnPos]->color != pieceToMove->color) {
            if (leftPawnPos % 8 != 7) {
                board[leftPawnPos]->pFlags->ENPASSANTRIGHT = false;
            }
        }
        if (board[rightPawnPos] != nullptr && board[rightPawnPos]->name == PieceType::PAWN && board[rightPawnPos]->color != pieceToMove->color) {
            if (rightPawnPos % 8 != 0) {
                board[rightPawnPos]->pFlags->ENPASSANTLEFT = false;
            }
        }
    }

    // Handle promotion
    if (move.isPromotion) {
        // Replace the promoted piece with the original pawn
        pieceToMove = new Pawn(pieceToMove->color, pawns[turn].back());
        board[to] = pieceToMove;
        pawns[turn].pop_back();
    }

    // Update the piece's position
    pieceToMove->position = from;

    moveGenerator->GenerateCapturesOnly();
    // Update the turn
    turn = pieceToMove->color;
    gameLoop();
}

void Board::removePiece(int position) {
    Piece* piece = board[position];



    board[position] = nullptr;
}

void Board::promotePawn(Piece* pawn) {}