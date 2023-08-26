#include <iostream>
#include "GameLoop.h"

const int boardArray[8][8] = {
        {0, 1, 2, 3, 4, 5, 6, 7},
        {8, 9, 10, 11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20, 21, 22, 23},
        {24, 25, 26, 27, 28, 29, 30, 31},
        {32, 33, 34, 35, 36, 37, 38, 39},
        {40, 41, 42, 43, 44, 45, 46, 47},
        {48, 49, 50, 51, 52, 53, 54, 55},
        {56, 57, 58, 59, 60, 61, 62, 63}
};

GameLoop::GameLoop(Board* board) {
    this->board = board;
}

GameLoop::~GameLoop() = default;

void GameLoop::gameLoop() {
    board->inCheck = false;
    board->movesThatBlockCheck.clear();
    board->amountOfChecks = 0;
    board->knightChecked = false;
    attackingPieces.clear();
    pinnedPieces.clear();
    checkForCheck();
    checkForCheckmate();
}

void GameLoop::checkForCheck() {
    // Get the king
    Piece* king = board->pieces[board->kings.at(board->turn)];

    // A function that checks if a piece is a threat to the king
    auto checkForThreat = [&](int pos, PieceType threatType) {
        if (board->board[pos] != nullptr) {
            Piece *piece = board->board[pos];
            if (piece->color == king->color) {
                return false;
            }
            if (piece->name == threatType) {
                attackingPieces.push_back(piece);
                board->inCheck = true;
                board->amountOfChecks++;
                if (piece->name == PieceType::KNIGHT) {
                    board->knightChecked = true;
                }
                return true;
            }
        }
        return false;
    };

    // Check for threats use a loop for each diagonal, horizontal, and vertical starting from the king
    const int kingPos = king->position;
    const int kingRow = kingPos / 8;
    const int kingCol = kingPos % 8;

    // Check if the king is in check by a pawn
    if (king->color == ChessColor::COLORBLACK) {
        if (kingPos + 7 < 64 && kingCol != 7) {
            if (board->board[kingPos + 7] != nullptr) {
                if (board->board[kingPos + 7]->color != king->color &&
                    board->board[kingPos + 7]->name == PieceType::PAWN) {
                    attackingPieces.push_back(board->board[kingPos + 7]);
                    board->inCheck = true;
                    board->amountOfChecks++;
                }
            }
        }
        if (kingPos + 9 < 64 && kingCol != 0) {
            if (board->board[kingPos + 9] != nullptr) {
                if (board->board[kingPos + 9]->color != king->color &&
                    board->board[kingPos + 9]->name == PieceType::PAWN) {
                    attackingPieces.push_back(board->board[kingPos + 9]);
                    board->inCheck = true;
                    board->amountOfChecks++;
                }
            }
        }
    } else {
        if (kingPos - 7 >= 0 && kingCol != 0) {
            if (board->board[kingPos - 7] != nullptr) {
                if (board->board[kingPos - 7]->color != king->color &&
                    board->board[kingPos - 7]->name == PieceType::PAWN) {
                    attackingPieces.push_back(board->board[kingPos - 7]);
                    board->inCheck = true;
                    board->amountOfChecks++;
                }
            }
        }
        if (kingPos - 9 >= 0 && kingCol != 7) {
            if (board->board[kingPos - 9] != nullptr) {
                if (board->board[kingPos - 9]->color != king->color &&
                    board->board[kingPos - 9]->name == PieceType::PAWN) {
                    attackingPieces.push_back(board->board[kingPos - 9]);
                    board->inCheck = true;
                    board->amountOfChecks++;
                }
            }
        }
    }

    // Diagonal
    if (kingCol != 7 && kingRow != 7) {
        for (int i = kingPos; i < 64; i += 7) {
            if (board->board[i] != nullptr) {
                if (board->board[i]->color == king->color && i != kingPos) {
                    pinnedPieces.emplace(board->board[i], Directions::DIAGONALDOWNRIGHT);
                    break;
                }
                if (checkForThreat(i, PieceType::BISHOP) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }
            if (i % 8 == 0) {
                break;
            }

        }
    }

    if (kingCol != 0 && kingRow != 7) {
        for (int i = kingPos; i < 64; i += 9) {
            if (board->board[i] != nullptr) {
                if (board->board[i]->color == king->color && i != kingPos) {
                    pinnedPieces.emplace(board->board[i], Directions::DIAGONALUPLEFT);
                    break;
                }
                if (checkForThreat(i, PieceType::BISHOP) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }

            if (i % 8 == 7) {
                break;
            }
        }
    }

    if (kingCol != 7 && kingRow != 0) {
        for (int i = kingPos; i >= 0; i -= 7) {
            if (board->board[i] != nullptr) {
                if (board->board[i]->color == king->color && i != kingPos) {
                    pinnedPieces.emplace(board->board[i], Directions::DIAGONALUPRIGHT);
                    break;
                }
                if (checkForThreat(i, PieceType::BISHOP) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }

            if (i % 8 == 7) {
                break;
            }
        }
    }

    if (kingCol != 0 && kingRow != 0) {
        for (int i = kingPos; i >= 0; i -= 9) {
            if (board->board[i] != nullptr) {
                if (board->board[i]->color == king->color && i != kingPos) {
                    pinnedPieces.emplace(board->board[i], Directions::DIAGONALDOWNLEFT);
                    break;
                }
                if (checkForThreat(i, PieceType::BISHOP) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }

            if (i % 8 == 0) {
                break;
            }
        }
    }

    // Horizontal
    if (kingCol != 7) {
        for (int i = kingPos; i < 64; i++) {
            if (board->board[i] != nullptr) {
                if (board->board[i]->color == king->color && i != kingPos) {
                    pinnedPieces.emplace(board->board[i], Directions::HORIZONTALRIGHT);
                    break;
                }
                if (checkForThreat(i, PieceType::ROOK) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }

            if (i % 8 == 7) {
                break;
            }
        }
    }

    if (kingCol != 0) {
        for (int i = kingPos; i >= 0; i--) {
            if (board->board[i] != nullptr) {
                if (board->board[i]->color == king->color && i != kingPos) {
                    pinnedPieces.emplace(board->board[i], Directions::HORIZONTALLEFT);
                    break;
                }
                if (checkForThreat(i, PieceType::ROOK) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }

            if (i % 8 == 0) {
                break;
            }
        }
    }

    // Vertical
    if (kingRow != 0) {
        for (int i = kingPos; i < 64; i += 8) {
            if (board->board[i] != nullptr) {
                if (board->board[i]->color == king->color && i != kingPos) {
                    pinnedPieces.emplace(board->board[i], Directions::VERTICALUP);
                    break;
                }
                if (checkForThreat(i, PieceType::ROOK) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }

        }
    }

    if (kingRow != 7) {
        for (int i = kingPos; i >= 0; i -= 8) {
            if (board->board[i] != nullptr) {
                if (board->board[i]->color == king->color && i != kingPos) {
                    pinnedPieces.emplace(board->board[i], Directions::VERTICALDOWN);
                    break;
                }
                if (checkForThreat(i, PieceType::ROOK) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }
        }
    }

    // Knight
    if (kingPos + 17 < 64) {
        checkForThreat(kingPos + 17, PieceType::KNIGHT);
    }
    if (kingPos + 15 < 64) {
        checkForThreat(kingPos + 15, PieceType::KNIGHT);
    }
    if (kingPos + 10 < 64) {
        checkForThreat(kingPos + 10, PieceType::KNIGHT);
    }
    if (kingPos + 6 < 64) {
        checkForThreat(kingPos + 6, PieceType::KNIGHT);
    }
    if (kingPos - 6 >= 0) {
        checkForThreat(kingPos - 6, PieceType::KNIGHT);
    }
    if (kingPos - 10 >= 0) {
        checkForThreat(kingPos - 10, PieceType::KNIGHT);
    }
    if (kingPos - 15 >= 0) {
        checkForThreat(kingPos - 15, PieceType::KNIGHT);
    }
    if (kingPos - 17 >= 0) {
        checkForThreat(kingPos - 17, PieceType::KNIGHT);
    }

    getPinnedPieces();
    if (board->inCheck) {
       generatePotentialMoves();
    }
}

void GameLoop::generatePotentialMoves() {
    if (board->amountOfChecks > 1) {
        return;
    } else if (board->knightChecked) {
        for (const auto& piece : attackingPieces) {
            if (piece == nullptr) {
                continue;
            } else if (piece->name == PieceType::KNIGHT && piece->color != board->turn) {
                board->movesThatBlockCheck.push_back(piece->position);
            }
        }
        return;
    }

    // Get the king
    Piece* king = board->pieces[board->kings.at(board->turn)];

    for (auto attacker : attackingPieces) {
        board->movesThatBlockCheck.push_back(attacker->position);
        std::vector<int> positions = getPositionsBetween(attacker->position, king->position);
        board->movesThatBlockCheck.insert(board->movesThatBlockCheck.end(), positions.begin(), positions.end());
    }
}


std::vector<int> GameLoop::getPositionsBetween(int startPos, int endPos) {
    std::vector<int> positions;

    // Make sure the positions are valid (between 0 and 63)
    if (startPos < 0 || startPos > 63 || endPos < 0 || endPos > 63) {
        return positions;
    }

    // Determine the direction of movement
    int dx = (endPos % 8) - (startPos % 8);
    int dy = (endPos / 8) - (startPos / 8);

    // Check if positions are on the same row, column, or diagonal
    if (dx == 0 || dy == 0 || std::abs(dx) == std::abs(dy)) {
        int stepX = (dx > 0) - (dx < 0);
        int stepY = (dy > 0) - (dy < 0);

        int currentPos = startPos + stepX + stepY * 8;
        while (currentPos != endPos) {
            positions.push_back(currentPos);
            currentPos += stepX + stepY * 8;
        }
    }

    return positions;
}

void GameLoop::getPinnedPieces() {
    for (auto pinnedPiece : pinnedPieces) {
        switch (pinnedPiece.second) {
            case Directions::DIAGONALUPLEFT:
                for (int i = pinnedPiece.first->position; i < 64; i -= 9) {
                    if (board->board[i] != nullptr) {
                        if (board->board[i]->color == pinnedPiece.first->color && i != pinnedPiece.first->position) {
                            break;
                        }
                        if (board->board[i]->color != pinnedPiece.first->color && (board->board[i]->name == PieceType::BISHOP || board->board[i]->name == PieceType::QUEEN)) {
                            pinnedPiece.first->pFlags->PINNED = true;
                            pinnedPiece.first->pinDirection = Directions::DIAGONALUPLEFT;
                            board->pinnedPieces.push_back(pinnedPiece.first);
                            break;
                        }
                    }
                    if (i % 8 == 0) {
                        break;
                    }
                }
                break;
            case Directions::DIAGONALUPRIGHT:
                for (int i = pinnedPiece.first->position; i < 64; i -= 7) {
                    if (board->board[i] != nullptr) {
                        if (board->board[i]->color == pinnedPiece.first->color && i != pinnedPiece.first->position) {
                            break;
                        }
                        if (board->board[i]->color != pinnedPiece.first->color && (board->board[i]->name == PieceType::BISHOP || board->board[i]->name == PieceType::QUEEN)) {
                            pinnedPiece.first->pFlags->PINNED = true;
                            pinnedPiece.first->pinDirection = Directions::DIAGONALUPRIGHT;
                            board->pinnedPieces.push_back(pinnedPiece.first);
                            break;
                        }
                    }
                    if (i % 8 == 7) {
                        break;
                    }
                }
                break;
            case Directions::DIAGONALDOWNLEFT:
                for (int i = pinnedPiece.first->position; i < 64; i += 7) {
                    if (board->board[i] != nullptr) {
                        if (board->board[i]->color == pinnedPiece.first->color && i != pinnedPiece.first->position) {
                            break;
                        }
                        if (board->board[i]->color != pinnedPiece.first->color && (board->board[i]->name == PieceType::BISHOP || board->board[i]->name == PieceType::QUEEN)) {
                            pinnedPiece.first->pFlags->PINNED = true;
                            pinnedPiece.first->pinDirection = Directions::DIAGONALDOWNLEFT;
                            board->pinnedPieces.push_back(pinnedPiece.first);
                            break;
                        }
                    }
                    if (i % 8 == 0) {
                        break;
                    }
                }
                break;
            case Directions::DIAGONALDOWNRIGHT:
                for (int i = pinnedPiece.first->position; i < 64; i += 9) {
                    if (board->board[i] != nullptr) {
                        if (board->board[i]->color == pinnedPiece.first->color && i != pinnedPiece.first->position) {
                            break;
                        }
                        if (board->board[i]->color != pinnedPiece.first->color && (board->board[i]->name == PieceType::BISHOP || board->board[i]->name == PieceType::QUEEN)) {
                            pinnedPiece.first->pFlags->PINNED = true;
                            pinnedPiece.first->pinDirection = Directions::DIAGONALDOWNRIGHT;
                            board->pinnedPieces.push_back(pinnedPiece.first);
                            break;
                        }
                    }
                    if (i % 8 == 7) {
                        break;
                    }
                }
                break;
            case Directions::HORIZONTALLEFT:
                for (int i = pinnedPiece.first->position; i >= 0; i--) {
                    if (board->board[i] != nullptr) {
                        if (board->board[i]->color == pinnedPiece.first->color && i != pinnedPiece.first->position) {
                            break;
                        }
                        if (board->board[i]->color != pinnedPiece.first->color && (board->board[i]->name == PieceType::ROOK || board->board[i]->name == PieceType::QUEEN)) {
                            pinnedPiece.first->pFlags->PINNED = true;
                            pinnedPiece.first->pinDirection = Directions::HORIZONTALLEFT;
                            board->pinnedPieces.push_back(pinnedPiece.first);
                            break;
                        }
                    }
                    if (i % 8 == 0) {
                        break;
                    }
                }
                break;
            case Directions::HORIZONTALRIGHT:
                for (int i = pinnedPiece.first->position; i < 64; i++) {
                    if (board->board[i] != nullptr) {
                        if (board->board[i]->color == pinnedPiece.first->color && i != pinnedPiece.first->position) {
                            break;
                        }
                        if (board->board[i]->color != pinnedPiece.first->color && (board->board[i]->name == PieceType::ROOK || board->board[i]->name == PieceType::QUEEN)) {
                            pinnedPiece.first->pFlags->PINNED = true;
                            pinnedPiece.first->pinDirection = Directions::HORIZONTALRIGHT;
                            board->pinnedPieces.push_back(pinnedPiece.first);
                            break;
                        }
                    }
                    if (i % 8 == 7) {
                        break;
                    }
                }
                break;
            case Directions::VERTICALUP:
                for (int i = pinnedPiece.first->position; i < 64; i += 8) {
                    if (board->board[i] != nullptr) {
                        if (board->board[i]->color == pinnedPiece.first->color && i != pinnedPiece.first->position) {
                            break;
                        }
                        if (board->board[i]->color != pinnedPiece.first->color && (board->board[i]->name == PieceType::ROOK || board->board[i]->name == PieceType::QUEEN)) {
                            pinnedPiece.first->pFlags->PINNED = true;
                            pinnedPiece.first->pinDirection = Directions::VERTICALUP;
                            board->pinnedPieces.push_back(pinnedPiece.first);
                            break;
                        }
                    }
                }
                break;
            case Directions::VERTICALDOWN:
                for (int i = pinnedPiece.first->position; i >= 0; i -= 8) {
                    if (board->board[i] != nullptr) {
                        if (board->board[i]->color == pinnedPiece.first->color && i != pinnedPiece.first->position) {
                            break;
                        }
                        if (board->board[i]->color != pinnedPiece.first->color && (board->board[i]->name == PieceType::ROOK || board->board[i]->name == PieceType::QUEEN)) {
                            pinnedPiece.first->pFlags->PINNED = true;
                            pinnedPiece.first->pinDirection = Directions::VERTICALDOWN;
                            board->pinnedPieces.push_back(pinnedPiece.first);
                            break;
                        }
                    }
                }
                break;
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

bool GameLoop::checkIfPieceProtected(Piece* p) {

    if (std::find(board->controlledSquares.begin(), board->controlledSquares.end(), p->position) != board->controlledSquares.end()) {
        return true;
    }
    return false;
}