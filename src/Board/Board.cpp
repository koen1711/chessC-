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
    moveGeneratorClass = new MoveGenerator(this);
    init();
}

void Board::init()
{
    Setup setup;
    setup.addFenToSetup("rnbqkbnr/pppppppppp/PPPPPPPP");

    for (const auto& pair : setup.pieces) {
        std::string piece = pair.first;
        const auto& positions = pair.second;
        for (int position : positions) {
            if (piece == "p") {
                board[position] = new Pawn(position, "black", 'p', *this);
                pawns["black"].push_back(board[position]);
            } else if (piece == "P") {
                board[position] = new Pawn(position, "white", 'P', *this);
                pawns["white"].push_back(board[position]);
            } else if (piece == "r") {
                board[position] = new Rook(position, "black", 'r', *this);
                rooks["black"].push_back(board[position]);
            } else if (piece == "R") {
                board[position] = new Rook(position, "white", 'R', *this);
                rooks["white"].push_back(board[position]);
            } else if (piece == "n") {
                board[position] = new Knight(position, "black", 'n', *this);
                knights["black"].push_back(board[position]);
            } else if (piece == "N") {
                board[position] = new Knight(position, "white", 'N', *this);
                knights["white"].push_back(board[position]);
            } else if (piece == "b") {
                board[position] = new Bishop(position, "black", 'b', *this);
                bishops["black"].push_back(board[position]);
            } else if (piece == "B") {
                board[position] = new Bishop(position, "white", 'B', *this);
                bishops["white"].push_back(board[position]);
            } else if (piece == "q") {
                board[position] = new Queen(position, "black", 'q', *this);
                queens["black"].push_back(board[position]);
            } else if (piece == "Q") {
                board[position] = new Queen(position, "white", 'Q', *this);
                queens["white"].push_back(board[position]);
            } else if (piece == "k") {
                board[position] = new King(position, "black", 'k', *this);
                kings["black"].push_back(board[position]);
                this->blackKing = board[position];
            } else if (piece == "K") {
                board[position] = new King(position, "white", 'K', *this);
                kings["white"].push_back(board[position]);
                this->whiteKing = board[position];
            }
        }
    }
    moveGeneratorClass->GenerateMoves();
}


void Board::gameLoop() {
    movesThatBlockCheck.clear();
    inCheck = false;
    if (gameLoopClass->checkForCheck(board)) {
        gameLoopClass->generateMovesThatBlockCheck(board);
        inCheck = true;
        if (gameLoopClass->checkForCheckmate(board)) {
        }
    }
    moveGeneratorClass->GenerateMoves();
}

void Board::movePiece(const Move &move) {
    int from = move.fromSquare;
    int to = move.toSquare;
    Piece* pieceToMove = board[from];

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

        // Move the rook
        board[rookTo] = board[rookFrom];
        board[rookFrom] = nullptr;
    }

    // Handle en passant
    if (move.isEnPassant) {
        int capturedPawnPos;
        if (turn == "white") {
            capturedPawnPos = to + 8;
        } else {
            capturedPawnPos = to - 8;
        }
        board[capturedPawnPos] = move.capturedPiece;
        removePiece(capturedPawnPos);
    } else {
        board[to] = move.capturedPiece;
        if (board[to] != nullptr) {
            // Capture the piece at the destination square
            removePiece(to);
        }
    }

    // Move the piece from 'from' to 'to'
    board[to] = board[from];
    board[from] = nullptr;

    // Update the piece's position
    pieceToMove->position = to;

    // Handle promotion
    if (move.isPromotion) {
        promotePawn(pieceToMove);
        pieceToMove = board[to]; // The promoted pawn is replaced with the new piece
    }

    // Update the piece's moved status
    board[to] = pieceToMove;
    board[from] = nullptr;
    turn = (turn == "white") ? "black" : "white";
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
        if (turn == "white") {
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
    turn = (turn == "white") ? "black" : "white";
}

void Board::removePiece(int position) {
    Piece* piece = board[position];
    std::string color = piece->color;

    // find the piece in the appropriate vector and remove it
    if (piece->name == "pawn") {
        if (pawns[color].size() > 0) {
            auto it = std::find(pawns[color].begin(), pawns[color].end(), piece);
            if (it != pawns[color].end()) {
                pawns[color].erase(it);
            }
        }
    } else if (piece->name == "knight") {
        if (knights[color].size() > 0) {
            auto it = std::find(knights[color].begin(), knights[color].end(), piece);
            if (it != knights[color].end()) {
                knights[color].erase(it);
            }
        }
    } else if (piece->name == "bishop") {
        if (bishops[color].size() > 0) {
            auto it = std::find(bishops[color].begin(), bishops[color].end(), piece);
            if (it != bishops[color].end()) {
                bishops[color].erase(it);
            }
        }
    } else if (piece->name == "rook") {
        if (rooks[color].size() > 0) {
            auto it = std::find(rooks[color].begin(), rooks[color].end(), piece);
            if (it != rooks[color].end()) {
                rooks[color].erase(it);
            }
        }
    } else if (piece->name == "queen") {
        if (queens[color].size() > 0) {
            auto it = std::find(queens[color].begin(), queens[color].end(), piece);
            if (it != queens[color].end()) {
                queens[color].erase(it);
            }
        }
    } else if (piece->name == "king") {
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
    int y = mousePosition.y / 100; // Reverse the y-coordinate to match the board orientation
    int position = x + y * 8;

    Piece* piece = board[position];
    if (piece != nullptr && piece->color == this->turn) {
        if (piece->color != this->turn)
            return;
        std::map<int, Move*> moves = piece->getMoves();
        this->potentialMoves = moves;
        this->activePiece = piece;
    } else if (this->activePiece != nullptr) {
        Move* move = potentialMoves[position];
        if (move == nullptr)
            return;
        movePiece(*move);
    }
}
