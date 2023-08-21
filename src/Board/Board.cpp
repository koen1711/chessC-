#include "Board.h"
#include "Setup/Setup.h"
#include <algorithm>
#include <chrono>
#include <iostream>

#define COLOR_WHITE (Color){255, 255, 255, 255}
#define COLOR_BLACK DARKBROWN

Board::Board()
{
    gameLoopClass = new GameLoop(this);
    moveGenerator = new MoveGenerator(this);
    this->init();
}

void Board::init()
{
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
                pawns[ChessColor::COLORWHITE].push_back(pieceToPush);
            } else if (piece == "P") {
                pieceToPush = new Pawn(ChessColor::COLORBLACK, position);
                pawns[ChessColor::COLORBLACK].push_back(pieceToPush);
            } else if (piece == "r") {
                pieceToPush = new Rook(ChessColor::COLORWHITE, position);
                rooks[ChessColor::COLORWHITE].push_back(pieceToPush);
            } else if (piece == "R") {
                pieceToPush = new Rook(ChessColor::COLORBLACK, position);
                rooks[ChessColor::COLORBLACK].push_back(pieceToPush);
            } else if (piece == "n") {
                pieceToPush = new Knight(ChessColor::COLORWHITE, position);
                knights[ChessColor::COLORWHITE].push_back(pieceToPush);
            } else if (piece == "N") {
                pieceToPush = new Knight(ChessColor::COLORBLACK, position);
                knights[ChessColor::COLORBLACK].push_back(pieceToPush);
            } else if (piece == "b") {
                pieceToPush = new Bishop(ChessColor::COLORWHITE, position);
                bishops[ChessColor::COLORWHITE].push_back(pieceToPush);
            } else if (piece == "B") {
                pieceToPush = new Bishop(ChessColor::COLORBLACK, position);
                bishops[ChessColor::COLORBLACK].push_back(pieceToPush);
            } else if (piece == "q") {
                pieceToPush = new Queen(ChessColor::COLORWHITE, position);
                queens[ChessColor::COLORWHITE].push_back(pieceToPush);
            } else if (piece == "Q") {
                pieceToPush = new Queen(ChessColor::COLORBLACK, position);
                queens[ChessColor::COLORBLACK].push_back(pieceToPush);
            } else if (piece == "k") {
                pieceToPush = new King(ChessColor::COLORWHITE, position);
                kings[ChessColor::COLORWHITE].push_back(pieceToPush);
            } else if (piece == "K") {
                pieceToPush = new King(ChessColor::COLORBLACK, position);
                kings[ChessColor::COLORBLACK].push_back(pieceToPush);
            }

            board[position] = pieceToPush;
        }
    }
    moveGenerator->GenerateMoves();
    std::cout << "Board initialized" << std::endl;
}


void Board::gameLoop() {
    movesThatBlockCheck.clear();
    inCheck = false;
    gameLoopClass->gameLoop();
    if (inCheck) {
        inCheck = true;
        if (checkmate) {
            std::cout << "Checkmate" << std::endl;
        }
    }
    moveGenerator->GenerateMoves();
}

void Board::movePiece(Move* move) {
    int from = move->fromSquare;
    int to = move->toSquare;
    Piece* pieceToMove = board[from];

    // Handle castling separately
    if (move->isCastle) {
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

        // Move the rook
        board[rookTo] = board[rookFrom];
        board[rookFrom] = nullptr;
    }

    // Handle en passant
    if (move->isEnPassant) {
        int capturedPawnPos;
        if (turn == ChessColor::COLORWHITE) {
            capturedPawnPos = to + 8;
        } else {
            capturedPawnPos = to - 8;
        }
        board[capturedPawnPos] = move->capturedPiece;
        removePiece(capturedPawnPos);
    } else {
        board[to] = move->capturedPiece;
        if (board[to] != nullptr) {
            // Capture the piece at the destination square
            removePiece(to);
        }
    }



    // Update the piece's position
    pieceToMove->position = to;

    // Handle promotion
    if (move->isPromotion) {
        promotePawn(pieceToMove);
        pieceToMove = board[to]; // The promoted pawn is replaced with the new piece
    }

    // Update the piece's moved status
    board[to] = pieceToMove;
    board[from] = nullptr;
    pieceToMove->moves.clear();
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
        std::cout << "Captured piece: " << move.capturedPiece->name << std::endl;
        // Restore the captured piece if there was any
        move.capturedPiece->position = to;
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

    // Handle promotion
    if (move.isPromotion) {
        // Replace the promoted piece with the original pawn
        pieceToMove = pawns[turn].back();
        board[to] = pieceToMove;
        pawns[turn].pop_back();
    }

    // Update the piece's position
    pieceToMove->position = from;

    // Update the turn
    turn = pieceToMove->color;
}

void Board::removePiece(int position) {
    Piece* piece = board[position];
    ChessColor color = piece->color;

    // find the piece in the appropriate vector and remove it
    if (piece->name == PieceType::PAWN) {
        if (pawns[color].size() > 0) {
            auto it = std::find(pawns[color].begin(), pawns[color].end(), piece);
            if (it != pawns[color].end()) {
                pawns[color].erase(it);
            }
        }
    } else if (piece->name == PieceType::KNIGHT) {
        if (knights[color].size() > 0) {
            auto it = std::find(knights[color].begin(), knights[color].end(), piece);
            if (it != knights[color].end()) {
                knights[color].erase(it);
            }
        }
    } else if (piece->name == PieceType::BISHOP) {
        if (bishops[color].size() > 0) {
            auto it = std::find(bishops[color].begin(), bishops[color].end(), piece);
            if (it != bishops[color].end()) {
                bishops[color].erase(it);
            }
        }
    } else if (piece->name == PieceType::ROOK) {
        if (rooks[color].size() > 0) {
            auto it = std::find(rooks[color].begin(), rooks[color].end(), piece);
            if (it != rooks[color].end()) {
                rooks[color].erase(it);
            }
        }
    } else if (piece->name == PieceType::QUEEN) {
        if (queens[color].size() > 0) {
            auto it = std::find(queens[color].begin(), queens[color].end(), piece);
            if (it != queens[color].end()) {
                queens[color].erase(it);
            }
        }
    } else if (piece->name == PieceType::KING) {
        if (kings[color].size() > 0) {
            auto it = std::find(kings[color].begin(), kings[color].end(), piece);
            if (it != kings[color].end()) {
                kings[color].erase(it);
            }
        }
    }

    board[position] = nullptr;
}

void Board::promotePawn(Piece* pawn) {}

void Board::handleClick(Vector2 mousePosition) {
    int x = mousePosition.x / 100;
    int y = mousePosition.y / 100;
    int position = x + y * 8;

    Piece* piece = board[position];
    if (piece != nullptr && piece->color == this->turn) {
        if (piece->color != this->turn)
            return;
        this->potentialMoves = piece->moves;
        this->activePiece = piece;
    } else if (this->activePiece != nullptr) {
        Move* move = potentialMoves[position];
        if (move == nullptr)
            return;
        movePiece(move);
        potentialMoves.clear();
        gameLoop();
    }
}
