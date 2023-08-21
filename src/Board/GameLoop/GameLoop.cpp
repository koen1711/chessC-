#include "GameLoop.h"

GameLoop::GameLoop(Board* board) {
    this->board = board;
}

GameLoop::~GameLoop() = default;

void GameLoop::gameLoop() {
    getPinnedPieces();
    checkForCheck();
    checkForCheckmate();
}

void GameLoop::checkForCheck() {
    if (board->moveGenerator->moveSize == 0) {
        board->checkmate = true;
    }
}

void GameLoop::getPinnedPieces() {
    // go through all pieces
    for (int i = 0; i < 64; i++) {
        if (board->board[i] != nullptr) {
            // if the piece is the same color as the current turn
            if (board->board[i]->color == board->turn) {
                // go through all moves
                for (auto move : board->board[i]->moves) {
                    // if the move is a capture
                    if (move.second->capturedPiece != nullptr) {
                        // if the piece is pinned
                        if (move.second->capturedPiece->name == PieceType::KING) {
                            board->board[i]->pinned = true;
                        }
                    }
                }
            }
        }
    }
}

void GameLoop::checkForCheckmate() {
    if (board->inCheck) {
        if (board->moveGenerator->moveSize == 0) {
            board->checkmate = true;
        }
    }
}