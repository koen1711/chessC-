#include <iostream>
#include "GameLoop.h"

GameLoop::GameLoop(Board* board) {
    this->board = board;
}

GameLoop::~GameLoop() = default;

void GameLoop::gameLoop() {
    board->inCheck = false;
    board->checkmate = false;
    board->movesThatBlockCheckBitboard = 0ULL;
    board->amountOfChecks = 0;
    board->knightChecked = false;
    attackingPieces = 0ULL;
    checkForCheck();
    checkForCheckmate();
}

void GameLoop::checkForCheck() {
    // Get the king
    int king;
    ChessColor kingColor = ChessColor::COLORNONE;
    if (board->turn == ChessColor::COLORWHITE) {
        king = board->whiteKing;
        kingColor = ChessColor::COLORWHITE;
    } else {
        king = board->blackKing;
        kingColor = ChessColor::COLORBLACK;
    }

    // A function that checks if a piece is a threat to the king
    auto checkForThreat = [&](int pos, PieceType threatType) {
        PieceType pieceType = board->getPieceType(pos);
        if (pieceType != NONE) {
            if (board->getPieceColor(pos) == kingColor) {
                return false;
            }
            if (pieceType == threatType) {
                attackingPieces |= 1ULL << pos;
                board->inCheck = true;
                board->amountOfChecks++;
                if (pieceType == PieceType::KNIGHT) {
                    board->knightChecked = true;
                }
                return true;
            }
        }
        return false;
    };

    // Check for threats use a loop for each diagonal, horizontal, and vertical starting from the king
    const int kingRow = king / 8;
    const int kingCol = king % 8;

    // Check if the king is in check by a pawn
    if (kingColor == ChessColor::COLORBLACK) {
        if (king + 7 < 64 && kingCol != 7) {
            if (board->getPieceType(king + 7) == PieceType::PAWN) {
                if (board->getPieceColor(king + 7) != kingColor) {
                    attackingPieces |= 1ULL << (king + 7);
                    board->inCheck = true;
                    board->amountOfChecks++;
                    std::cout << "King is in check by pawn at " << king + 7 << std::endl;
                }
            }
        }
        if (king + 9 < 64 && kingCol != 0) {
            if (board->getPieceType(king + 9) == PAWN) {
                if (board->getPieceColor(king + 9) != kingColor) {
                    attackingPieces |= 1ULL << (king + 9);
                    board->inCheck = true;
                    board->amountOfChecks++;
                    std::cout << "King is in check by pawn at " << king + 9 << std::endl;
                }
            }
        }
    } else {
        if (king - 7 >= 0 && kingCol != 0) {
            if (board->getPieceType(king - 7) == PAWN) {
                if (board->getPieceColor(king - 7) != kingColor) {
                    attackingPieces |= 1ULL << (king - 7);
                    board->inCheck = true;
                    board->amountOfChecks++;
                    std::cout << "King is in check by pawn at " << king - 7 << std::endl;
                }
            }
        }
        if (king - 9 >= 0 && kingCol != 7) {
            if (board->getPieceType(king - 9) == PAWN) {
                if (board->getPieceColor(king - 9) != kingColor) {
                    attackingPieces |= 1ULL << (king - 9);
                    board->inCheck = true;
                    board->amountOfChecks++;
                    std::cout << "King is in check by pawn at " << king - 9 << std::endl;
                }
            }
        }
    }

    // Diagonal
    if (kingCol != 7 && kingRow != 7) {
        for (int i = king; i < 64; i += 9) {
            if (board->getPieceType(i) != NONE) {
                if (board->getPieceColor(i) == kingColor && i != king) {
                    pinnedPieces.emplace(i, Directions::DIAGONALDOWNLEFT);
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
        for (int i = king; i < 64; i += 7) {
            if (board->getPieceType(i) != NONE) {
                if (board->getPieceColor(i) == kingColor && i != king) {
                    pinnedPieces.emplace(i, Directions::DIAGONALDOWNRIGHT);
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
        for (int i = king; i >= 0; i -= 7) {
            if (board->getPieceType(i) != NONE) {
                if (board->getPieceColor(i) == kingColor && i != king) {
                    pinnedPieces.emplace(i, Directions::DIAGONALUPRIGHT);
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
        for (int i = king; i >= 0; i -= 9) {
            if (board->getPieceType(i) != NONE) {
                if (board->getPieceColor(i) == kingColor && i != king) {
                    pinnedPieces.emplace(i, Directions::DIAGONALUPLEFT);
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
        for (int i = king; i < 64; i++) {
            if (board->getPieceType(i) != NONE) {
                if (board->getPieceColor(i) == kingColor && i != king) {
                    pinnedPieces.emplace(i, Directions::HORIZONTALRIGHT);
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
        for (int i = king; i >= 0; i--) {
            if (board->getPieceType(i) != NONE) {
                if (board->getPieceColor(i) == kingColor && i != king) {
                    pinnedPieces.emplace(i, Directions::HORIZONTALLEFT);
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
    if (kingRow < 7) {
        for (int i = king + 8; i < 64; i += 8) {
            if (board->getPieceType(i) != NONE) {
                if (board->getPieceColor(i) == kingColor && i != king) {
                    pinnedPieces.emplace(i, Directions::VERTICALUP);
                    break;
                }
                if (checkForThreat(i, PieceType::ROOK) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }

        }
    }

    if (kingRow > 0) {
        for (int i = king - 8; i >= 0; i -= 8) {
            if (board->getPieceType(i) != NONE) {
                if (board->getPieceColor(i) == kingColor) {
                    pinnedPieces.emplace(i, Directions::VERTICALDOWN);
                    break;
                }
                if (checkForThreat(i, PieceType::ROOK) || checkForThreat(i, PieceType::QUEEN)) {
                    break;
                }
            }
        }
    }

    // Knight
    if (king + 17 < 64) {
        checkForThreat(king + 17, PieceType::KNIGHT);
    }
    if (king + 15 < 64) {
        checkForThreat(king + 15, PieceType::KNIGHT);
    }
    if (king + 10 < 64) {
        checkForThreat(king + 10, PieceType::KNIGHT);
    }
    if (king + 6 < 64) {
        checkForThreat(king + 6, PieceType::KNIGHT);
    }
    if (king - 6 >= 0) {
        checkForThreat(king - 6, PieceType::KNIGHT);
    }
    if (king - 10 >= 0) {
        checkForThreat(king - 10, PieceType::KNIGHT);
    }
    if (king - 15 >= 0) {
        checkForThreat(king - 15, PieceType::KNIGHT);
    }
    if (king - 17 >= 0) {
        checkForThreat(king - 17, PieceType::KNIGHT);
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
        uint64_t attacks = this->attackingPieces;
        while (attacks) {
            int knight = __builtin_ctzll(attacks);
            if (board->getPieceType(knight) == PieceType::KNIGHT) {
                // Insert the knight's position into the moves that block check
                board->movesThatBlockCheckBitboard |= 1ULL << knight;
            }
            attacks &= attacks - 1;
        }
        return;
    }

    // Get the king

    int king;
    if (board->turn == ChessColor::COLORWHITE) {
        king = board->whiteKing;
    } else {
        king = board->blackKing;
    }

    uint64_t pieces = this->attackingPieces;
    while (pieces) {
        int attacker = __builtin_ctzll(pieces);
        board->movesThatBlockCheckBitboard |= 1ULL << attacker;
        uint64_t positions = getPositionsBetween(attacker, king);
        board->movesThatBlockCheckBitboard |= positions;
        pieces &= pieces - 1;
    }
}

uint64_t GameLoop::getPositionsBetween(int startPos, int endPos) {
    uint64_t positions = 0ULL;

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
            positions |= (1ULL << currentPos);
            currentPos += stepX + stepY * 8;
        }
    }

    return positions;
}

void GameLoop::getPinnedPieces() {
    board->pinnedPiecesBitboard = 0ULL;
    for (auto pinnedPiece : pinnedPieces) {
        ChessColor color = board->getPieceColor(pinnedPiece.first);
        switch (pinnedPiece.second) {
            case Directions::DIAGONALUPLEFT:
                for (int i = pinnedPiece.first - 9; i < 64; i -= 9) {
                    if (board->getPieceType(i) != NONE) {
                        if (board->getPieceColor(i) == color && i != pinnedPiece.first) {
                            break;
                        }
                        if (board->getPieceColor(i) != color && (board->getPieceType(i) == PieceType::BISHOP || board->getPieceType(i) == PieceType::QUEEN)) {
                            //pinnedPiece.first->pinDirection = Directions::DIAGONALUPLEFT;
                            board->pinnedPiecesBitboard |= 1ULL << pinnedPiece.first;
                            board->pinDirectionBitboards[Directions::DIAGONALUPLEFT] |= 1ULL << pinnedPiece.first;
                            break;
                        } else {
                            break;
                        }
                    }
                    if (i % 8 == 0) {
                        break;
                    }
                }
                break;
            case Directions::DIAGONALUPRIGHT:
                for (int i = pinnedPiece.first - 7; i < 64; i -= 7) {
                    if (board->getPieceType(i) != NONE) {
                        if (board->getPieceColor(i) == color && i != pinnedPiece.first) {
                            break;
                        }
                        if (board->getPieceColor(i) != color && (board->getPieceType(i) == PieceType::BISHOP || board->getPieceType(i) == PieceType::QUEEN)) {
                            //pinnedPiece.first->pinDirection = Directions::DIAGONALUPRIGHT;
                            board->pinnedPiecesBitboard |= 1ULL << pinnedPiece.first;
                            board->pinDirectionBitboards[Directions::DIAGONALUPRIGHT] |= 1ULL << pinnedPiece.first;
                            break;
                        } else {
                            break;
                        }
                    }
                    if (i % 8 == 7) {
                        break;
                    }
                }
                break;
            case Directions::DIAGONALDOWNRIGHT:
                for (int i = pinnedPiece.first + 7; i < 64; i += 7) {
                    if (board->getPieceType(i) != NONE) {
                        if (board->getPieceColor(i) == color && i != pinnedPiece.first) {
                            break;
                        }
                        if (board->getPieceColor(i) != color && (board->getPieceType(i) == PieceType::BISHOP || board->getPieceType(i) == PieceType::QUEEN)) {
                            //pinnedPiece.first->pinDirection = Directions::DIAGONALDOWNRIGHT;
                            board->pinnedPiecesBitboard |= 1ULL << pinnedPiece.first;
                            board->pinDirectionBitboards[Directions::DIAGONALDOWNRIGHT] |= 1ULL << pinnedPiece.first;
                            break;
                        } else {
                            break;
                        }
                    }
                    if (i % 8 == 0) {
                        break;
                    }
                }
                break;
            case Directions::DIAGONALDOWNLEFT:
                for (int i = pinnedPiece.first + 9; i < 64; i += 9) {
                    if (board->getPieceType(i) != NONE) {
                        if (board->getPieceColor(i) == color && i != pinnedPiece.first) {
                            break;
                        }
                        if (board->getPieceColor(i) != color && (board->getPieceType(i) == PieceType::BISHOP || board->getPieceType(i) == PieceType::QUEEN)) {
                            //pinnedPiece.first->pinDirection = Directions::DIAGONALDOWNLEFT;
                            board->pinnedPiecesBitboard |= 1ULL << pinnedPiece.first;
                            board->pinDirectionBitboards[Directions::DIAGONALDOWNLEFT] |= 1ULL << pinnedPiece.first;
                            break;
                        } else {
                            break;
                        }
                    }
                    if (i % 8 == 7) {
                        break;
                    }
                }
                break;
            case Directions::HORIZONTALLEFT:
                for (int i = pinnedPiece.first - 1; i >= 0; i--) {
                    if (board->getPieceType(i) != NONE) {
                        if (board->getPieceColor(i) == color && i != pinnedPiece.first) {
                            break;
                        }
                        if (board->getPieceColor(i) != color && (board->getPieceType(i) == PieceType::ROOK || board->getPieceType(i) == PieceType::QUEEN)) {
                            //pinnedPiece.first->pinDirection = Directions::HORIZONTALLEFT;
                            board->pinnedPiecesBitboard |= 1ULL << pinnedPiece.first;
                            board->pinDirectionBitboards[Directions::HORIZONTALLEFT] |= 1ULL << pinnedPiece.first;
                            break;
                        } else {
                            break;
                        }
                    }
                    if (i % 8 == 0) {
                        break;
                    }
                }
                break;
            case Directions::HORIZONTALRIGHT:
                for (int i = pinnedPiece.first + 1; i < 64; i++) {
                    if (board->getPieceType(i) != NONE) {
                        if (board->getPieceColor(i) == color && i != pinnedPiece.first) {
                            break;
                        }
                        if (board->getPieceColor(i) != color && (board->getPieceType(i) == PieceType::ROOK || board->getPieceType(i) == PieceType::QUEEN)) {
                            //pinnedPiece.first->pinDirection = Directions::HORIZONTALRIGHT;
                            board->pinnedPiecesBitboard |= 1ULL << pinnedPiece.first;
                            board->pinDirectionBitboards[Directions::HORIZONTALRIGHT] |= 1ULL << pinnedPiece.first;
                            break;
                        } else {
                            break;
                        }
                    }
                    if (i % 8 == 7) {
                        break;
                    }
                }
                break;
            case Directions::VERTICALUP:
                for (int i = pinnedPiece.first + 8; i < 64; i += 8) {
                    if (board->getPieceType(i) != NONE) {
                        if (board->getPieceColor(i) == color && i != pinnedPiece.first) {
                            break;
                        }
                        if (board->getPieceColor(i) != color && (board->getPieceType(i) == PieceType::ROOK || board->getPieceType(i) == PieceType::QUEEN)) {
                            //pinnedPiece.first->pinDirection = Directions::VERTICALUP;
                            board->pinnedPiecesBitboard |= 1ULL << pinnedPiece.first;
                            board->pinDirectionBitboards[Directions::VERTICALUP] |= 1ULL << pinnedPiece.first;
                            break;
                        } else {
                            break;
                        }
                    }
                }
                break;
            case Directions::VERTICALDOWN:
                for (int i = pinnedPiece.first - 8; i >= 0; i -= 8) {
                    if (board->getPieceType(i) != NONE) {
                        if (board->getPieceColor(i) == color && i != pinnedPiece.first) {
                            break;
                        }
                        if (board->getPieceColor(i) != color && (board->getPieceType(i) == PieceType::ROOK || board->getPieceType(i) == PieceType::QUEEN)) {
                            //pinnedPiece.first->pinDirection = Directions::VERTICALDOWN;
                            board->pinnedPiecesBitboard |= 1ULL << pinnedPiece.first;
                            board->pinDirectionBitboards[Directions::VERTICALDOWN] |= 1ULL << pinnedPiece.first;
                            break;
                        } else {
                            break;
                        }
                    }
                }
                break;
        }
    }
    pinnedPieces.clear();
}

void GameLoop::checkForCheckmate() {
    if (board->inCheck) {
        if (board->moveGenerator->moveSize == 0) {
            board->checkmate = true;
        }
    }
}

bool GameLoop::checkIfPieceProtected(int p) {

    if (board->controlledSquaresBitboard & (1ULL << p)) {
        return true;
    }
    return false;
}