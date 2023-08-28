#include "MoveGenerator.h"
#include <iostream>
#include <thread>

// boardrray 2d array
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

MoveGenerator::MoveGenerator(Board* board) : board(board) {};


void MoveGenerator::GenerateCapturesOnly() {
    board->controlledSquares.clear();
    std::vector<int> pawnCaptures = GeneratePawnCaptures();
    board->controlledSquares.insert(board->controlledSquares.end(), pawnCaptures.begin(), pawnCaptures.end());
    std::vector<int> knightCaptures = GenerateKnightCaptures();
    board->controlledSquares.insert(board->controlledSquares.end(), knightCaptures.begin(), knightCaptures.end());
    std::vector<int> bishopCaptures = GenerateBishopCaptures();
    board->controlledSquares.insert(board->controlledSquares.end(), bishopCaptures.begin(), bishopCaptures.end());
    std::vector<int> rookCaptures = GenerateRookCaptures();
    board->controlledSquares.insert(board->controlledSquares.end(), rookCaptures.begin(), rookCaptures.end());
    std::vector<int> queenCaptures = GenerateQueenCaptures();
    board->controlledSquares.insert(board->controlledSquares.end(), queenCaptures.begin(), queenCaptures.end());
    std::vector<int> kingCaptures = GenerateKingCaptures();
    board->controlledSquares.insert(board->controlledSquares.end(), kingCaptures.begin(), kingCaptures.end());
}

void MoveGenerator::GenerateMoves() {
    GeneratePawnMoves();
    GenerateKnightMoves();
    GenerateBishopMoves();
    GenerateRookMoves();
    GenerateQueenMoves();
    GenerateKingMoves();
}

void MoveGenerator::GeneratePawnMoves() {
    const bool blackTurn = board->turn == ChessColor::COLORBLACK;
    const int promotionRow = blackTurn ? 7 : 0;
    const int direction = blackTurn ? 1 : -1;
    const int startingRow = blackTurn ? 1 : 6;
    const int enPassantRight = blackTurn ? 9 : 7;
    const int enPassantLeft = blackTurn ? 7 : 9;

    // Loop through the Pawns bitboard
    uint64_t pawnsBitboard = board->pawnsBitboard;

    while (pawnsBitboard) {
        // Get the position of the first pawn
        int position = __builtin_ctzll(pawnsBitboard);
        // Remove the first pawn from the bitboard
        pawnsBitboard &= pawnsBitboard - 1ULL;
        // Generate the moves for the pawn
        // Get if this position is in the whitePiecesBitboard
        if (board->getPieceColor(position) == board->turn)
            GenerateOnePawnMoves(position, direction, startingRow, enPassantRight, enPassantLeft);
    }
}

void MoveGenerator::GenerateOnePawnMoves(int position, int direction, int startingRow, int enPassantRight, int enPassantLeft) const {
    std::vector<Move*> pawnMoves;
    int row = position / 8;
    int column = position % 8;

    // Check if pawn can move forward one square
    const int forwardOneSquare = boardArray[row + 1 * direction][column];
    if (board->getPieceType(forwardOneSquare) == NONE) {
        // Check if pawn can promote
        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), forwardOneSquare) != board->movesThatBlockCheck.end()) {
                pawnMoves.push_back(new Move(position, forwardOneSquare, board));
            }
        } else {
            pawnMoves.push_back(new Move(position, forwardOneSquare, board));
        }
        if (row == startingRow) {
            int forwardTwoSquares = position + (direction * 16);
            if (board->getPieceType(forwardTwoSquares) == NONE) {
                if (board->inCheck) {
                    if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), forwardTwoSquares) != board->movesThatBlockCheck.end()) {
                        pawnMoves.push_back(new Move(position, forwardTwoSquares, board));
                    }
                } else {
                    pawnMoves.push_back(new Move(position, forwardTwoSquares, board));
                }
            }
        }
    }
    if (board->enPassantRightBitboard & (1ULL << position)) {
        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantRight)) != board->movesThatBlockCheck.end()) {
                pawnMoves.push_back(new Move(position, position + (direction * enPassantRight), board));
            }
        } else {
            pawnMoves.push_back((new Move(position, position + (direction * enPassantRight), board)));
        }
    }
    if (board->enPassantLeftBitboard & (1ULL << position)) {
        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantLeft)) != board->movesThatBlockCheck.end()) {
                pawnMoves.push_back(new Move(position, position + (direction * enPassantLeft), board));
            }
        } else {
            pawnMoves.push_back(new Move(position, position + (direction * enPassantLeft), board));
        }
    }

    // check if we can capture a piece to the right
    if (column != 7) {
        int rightCapture = boardArray[row + (1 * direction)][column + 1];
        if (board->getPieceType(rightCapture) != NONE && board->getPieceColor(rightCapture) != board->turn) {
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), rightCapture) != board->movesThatBlockCheck.end()) {
                    pawnMoves.push_back(new Move(position, rightCapture, board));
                }
            } else {
                pawnMoves.push_back(new Move(position, rightCapture, board));
            }
        }
    }
    // check if we can capture a piece to the left
    if (column != 0) {
        int leftCapture = boardArray[row + 1 * direction][column - 1];
        if (board->getPieceType(leftCapture) != NONE && board->getPieceColor(leftCapture) != board->turn) {
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), leftCapture) != board->movesThatBlockCheck.end()) {
                    pawnMoves.push_back(new Move(position, leftCapture, board));
                }
            } else {
                pawnMoves.push_back(new Move(position, leftCapture, board));
            }
        }
    }

    if (board->enPassantLeftBitboard & (1ULL << position)) {
        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantLeft)) != board->movesThatBlockCheck.end()) {
                pawnMoves.push_back(new Move(position, position + (direction * enPassantLeft), board));
            }
        } else {
            pawnMoves.push_back((new Move(position, position + (direction * enPassantLeft), board)));
        }
    } else if (board->enPassantRightBitboard & (1ULL << position)) {
        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantRight)) != board->movesThatBlockCheck.end()) {
                pawnMoves.push_back(new Move(position, position + (direction * enPassantRight), board));
            }
        } else {
            pawnMoves.push_back((new Move(position, position + (direction * enPassantRight), board)));
        }
    }

    board->moveMap.emplace(position, pawnMoves);
}

void MoveGenerator::GenerateKnightMoves() {
    // Loop through the Knights bitboard
    uint64_t knightsBitboard = board->knightsBitboard;

    while (knightsBitboard) {
        // Get the position of the first knight
        int position = __builtin_ctzll(knightsBitboard);
        // Remove the first knight from the bitboard
        knightsBitboard &= knightsBitboard - 1ULL;
        // Generate the moves for the knight
        if (board->getPieceColor(position) == board->turn)
            GenerateOneKnightMoves(position);
    }
}

void MoveGenerator::GenerateOneKnightMoves(int position) const {
    std::vector<Move*> knightMoves = {};

    ChessColor color = board->getPieceColor(position);
    const int row = position / 8;
    const int column = position % 8;

    // Check if knight can move 2 rows up and 1 column right
    if (row < 6) {
        if (column < 7) {
            int move = position + 17;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
        // Check if knight can move 2 rows up and 1 column left
        if (column > 0) {
            int move = position + 15;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
    }
    // Check if knight can move 2 rows down and 1 column right
    if (row > 1) {
        if (column < 7) {
            int move = position - 15;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
        // Check if knight can move 2 rows down and 1 column left
        if (column > 0) {
            int move = position - 17;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
    }
    // Check if knight can move 1 row up and 2 columns right
    if (row < 7) {
        if (column < 6) {
            int move = position + 10;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
        // Check if knight can move 1 row up and 2 columns left
        if (column > 1) {
            int move = position + 6;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
    }
    // Check if knight can move 1 row down and 2 columns right
    if (row > 0) {
        if (column < 6) {
            int move = position - 6;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
        // Check if knight can move 1 row down and 2 columns left
        if (column > 1) {
            int move = position - 10;

            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
    }
    board->moveMap.emplace(position, knightMoves);
}

void MoveGenerator::GenerateRookMoves() {
    // Loop through the Rooks bitboard
    uint64_t rooksBitboard = board->rooksBitboard;

    while (rooksBitboard) {
        // Get the position of the first rook
        int position = __builtin_ctzll(rooksBitboard);
        // Remove the first rook from the bitboard
        rooksBitboard &= rooksBitboard - 1ULL;
        // Generate the moves for the rook
        if (board->getPieceColor(position) == board->turn)
            GenerateOneRookMoves(position);
    }
}

void MoveGenerator::GenerateOneRookMoves(int position) const {
    const int row = position / 8;
    const int column = position % 8;

    ChessColor color = board->getPieceColor(position);

    std::vector<Move*> rookMoves;

    // Check if rook can move up
    for (int i = row + 1; i < 8; i++) {
        int move = position + (8 * (i - row));

        if (board->getPieceType(move) != NONE && board->getPieceColor(move) == color) {
            break;
        }

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            rookMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != color) {
            rookMoves.push_back(new Move(position, move, board));
            break;
        }
  }
    // Check if rook can move down
    for (int i = row - 1; i >= 0; i--) {
        int move = position - (8 * (row - i));

        if (board->getPieceType(move) != NONE && board->getPieceColor(move) == color) {
            break;
        }

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            rookMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != color) {
            rookMoves.push_back(new Move(position, move, board));
            break;
        }
    }
    // Check if rook can move right
    for (int i = column + 1; i < 8; i++) {
        int move = position + (i - column);

        if (board->getPieceType(move) != NONE && board->getPieceColor(move) == color) {
            break;
        }

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            rookMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != color) {
            rookMoves.push_back(new Move(position, move, board));
            break;
        }
    }
    // Check if rook can move left
    for (int i = column - 1; i >= 0; i--) {
        int move = position - (column - i);

        if (board->getPieceType(move) != NONE && board->getPieceColor(move) == color) {
            break;
        }

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            rookMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != color) {
            rookMoves.push_back(new Move(position, move, board));
            break;
        }
    }

    board->moveMap.emplace(position, rookMoves);
}

void MoveGenerator::GenerateBishopMoves() {
    // Loop through the Bishops bitboard
    uint64_t bishopsBitboard = board->bishopsBitboard;

    while (bishopsBitboard) {
        // Get the position of the first bishop
        int position = __builtin_ctzll(bishopsBitboard);
        // Remove the first bishop from the bitboard
        bishopsBitboard &= bishopsBitboard - 1ULL;
        // Generate the moves for the bishop
        if (board->getPieceColor(position) == board->turn)
            GenerateOneBishopMoves(position);
    }
}

void MoveGenerator::GenerateOneBishopMoves( int position) const {
    const int row = position / 8;
    const int column = position % 8;

    std::vector<Move*> bishopMoves;

    // Check if bishop can move up and right
    for (int i = 1; i < 8; i++) {
        if (row + i > 7 || column + i > 7) {
            break;
        }
        int move = position + (9 * i);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            bishopMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            bishopMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if bishop can move up and left
    for (int i = 1; i < 8; i++) {
        if (row + i > 7 || column - i < 0) {
            break;
        }
        int move = position + (7 * i);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            bishopMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            bishopMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if bishop can move down and right
    for (int i = 1; i < 8; i++) {
        if (row - i < 0 || column + i > 7) {
            break;
        }
        int move = position - (7 * i);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            bishopMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            bishopMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if bishop can move down and left
    for (int i = 1; i < 8; i++) {
        if (row - i < 0 || column - i < 0) {
            break;
        }
        int move = position - (9 * i);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            bishopMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            bishopMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    board->moveMap.emplace(position, bishopMoves);
}

void MoveGenerator::GenerateQueenMoves() {
    // Loop through the Queens bitboard
    uint64_t queensBitboard = board->queensBitboard;

    while (queensBitboard) {
        // Get the position of the first queen
        int position = __builtin_ctzll(queensBitboard);
        // Remove the first queen from the bitboard
        queensBitboard &= queensBitboard - 1ULL;
        // Generate the moves for the queen
        if (board->getPieceColor(position) == board->turn)
            GenerateOneQueenMoves(position);
    }
}

void MoveGenerator::GenerateOneQueenMoves( int position) const {
    const int row = position / 8;
    const int column = position % 8;

    std::vector<Move*> queenMoves;

    // Check if queen can move up
    for (int i = row + 1; i < 8; i++) {
        int move = position + (8 * (i - row));

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            queenMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if queen can move down
    for (int i = row - 1; i >= 0; i--) {
        int move = position - (8 * (row - i));

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            queenMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if queen can move right
    for (int i = column + 1; i < 8; i++) {
        int move = position + (i - column);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            queenMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if queen can move left
    for (int i = column - 1; i >= 0; i--) {
        int move = position - (column - i);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            queenMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if queen can move up and right
    for (int i = 1; i < 8; i++) {
        if (row + i > 7 || column + i > 7) {
            break;
        }
        int move = position + (9 * i);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            queenMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if queen can move up and left
    for (int i = 1; i < 8; i++) {
        if (row + i > 7 || column - i < 0) {
            break;
        }
        int move = position + (7 * i);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            queenMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if queen can move down and right
    for (int i = 1; i < 8; i++) {
        if (row - i < 0 || column + i > 7) {
            break;
        }
        int move = position - (7 * i);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            queenMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    // Check if queen can move down and left
    for (int i = 1; i < 8; i++) {
        if (row - i < 0 || column - i < 0) {
            break;
        }
        int move = position - (9 * i);

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->getPieceColor(move) != board->turn) {
            queenMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    board->moveMap.emplace(position, queenMoves);
}

void MoveGenerator::GenerateKingMoves() {
    int king;
    if (board->turn == ChessColor::COLORWHITE) {
        king = board->whiteKing;
    } else {
        king = board->blackKing;
    }
    GenerateOneKingMoves(king);

}

void MoveGenerator::GenerateOneKingMoves( int position) const {
    std::vector<Move*> kingMoves;
    const int row = position / 8;
    const int column = position % 8;

    // Check if king can move 1 row up
    if (row < 7) {
        int move = position + 8;
        if (board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) {
            kingMoves.push_back(new Move(position, move, board));

        }
    }
    // Check if king can move 1 row down
    if (row > 0) {
        int move = position - 8;
        if (board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 column right
    if (column < 7) {
        int move = position + 1;
        if (board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 column left
    if (column > 0) {
        int move = position - 1;
        if (board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row up and 1 column right
    if (row < 7 && column < 7) {
        int move = position + 9;
        if (board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row up and 1 column left
    if (row < 7 && column > 0) {
        int move = position + 7;
        if (board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row down and 1 column right
    if (row > 0 && column < 7) {
        int move = position - 7;
        if (board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row down and 1 column left
    if (row > 0 && column > 0) {
        int move = position - 9;
        if (board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }

    if (!board->inCheck) {
        // Check if king can castle
        if (board->turn == ChessColor::COLORWHITE) {
            // check if there is a rook on the king side
            if (board->getPieceType(63) == PieceType::ROOK && board->getPieceColor(63) == ChessColor::COLORWHITE) {
                // check if there are no pieces between the king and the rook
                if (board->getPieceType(61) == NONE && board->getPieceType(61) == NONE && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 61) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 62) == board->controlledSquares.end()) {
                    kingMoves.push_back(new Move(position, 62, board));
                }
            }
            // check if there is a rook on the queen side
            if (board->getPieceType(56) == PieceType::ROOK && board->getPieceColor(56) == ChessColor::COLORWHITE) {
                // check if there are no pieces between the king and the rook
                if (board->getPieceType(58) == NONE && board->getPieceType(59) == NONE && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 57) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 58) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 59) == board->controlledSquares.end()) {
                    kingMoves.push_back(new Move(position, 58, board));
                }
            }

        } else {
            // check if there is a rook on the king side
            if (board->getPieceType(7) == PieceType::ROOK && board->getPieceColor(7) == ChessColor::COLORBLACK) {
                // check if there are no pieces between the king and the rook
                if (board->getPieceType(5) == NONE && board->getPieceType(6) == NONE && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 5) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 6) == board->controlledSquares.end()) {
                    kingMoves.push_back(new Move(position, 6, board));
                }
            }
            // check if there is a rook on the queen side
            if (board->getPieceType(0) == PieceType::ROOK && board->getPieceColor(0) == ChessColor::COLORBLACK) {
                // check if there are no pieces between the king and the rook
                if (board->getPieceType(1) == NONE && board->getPieceType(2) == NONE && board->getPieceType(3) == NONE && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 1) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 2) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 3) == board->controlledSquares.end()) {
                    kingMoves.push_back(new Move(position, 2, board));
                }
            }
        }
    }

    std::vector<Move*> validMoves;
    for (auto move : kingMoves) {
        if (std::find(board->controlledSquares.begin(), board->controlledSquares.end(), move->toSquare) == board->controlledSquares.end()) {
            validMoves.push_back(move);
        }
    }
    kingMoves = validMoves;

    board->moveMap.emplace(position, kingMoves);
}

std::vector<int> MoveGenerator::GeneratePawnCaptures() const {
    std::vector<int> pawnCaptures;

    uint64_t pawnsBitboard = board->pawnsBitboard;
    // Loop through the Pawns bitboard
    while (pawnsBitboard) {
        // Get the position of the first pawn
        int position = __builtin_ctzll(board->pawnsBitboard);
        if (board->getPieceColor(position) != board->turn) {
            pawnsBitboard &= pawnsBitboard - 1ULL;
            continue;
        }
        const int row = position / 8;
        const int column = position % 8;
        if (board->turn == ChessColor::COLORWHITE) {
            if (column > 0) {
                pawnCaptures.push_back(position - 7);
            }
            if (column < 7) {
                pawnCaptures.push_back(position - 9);
            }
        } else {
            if (column > 0) {
                pawnCaptures.push_back(position + 9);
            }
            if (column < 7) {
                pawnCaptures.push_back(position + 7);
            }
        }
        pawnsBitboard &= pawnsBitboard - 1ULL;
    }
    return pawnCaptures;
}

std::vector<int> MoveGenerator::GenerateKnightCaptures() const {
    std::vector<int> knightCaptures;

    uint64_t knightsBitboard = board->knightsBitboard;

    while (knightsBitboard) {
        const int position = __builtin_ctzll(knightsBitboard);
        if (board->getPieceColor(position) != board->turn) {
            knightsBitboard &= knightsBitboard - 1ULL;
            continue;
        }
        // Generate all moves, don't check if they are vali
        const int row = position / 8;
        const int column = position % 8;

        // Check if knight can move 2 rows up and 1 column right
        if (row < 6 && column < 7) {
            int move = position + 17;
            knightCaptures.push_back(move);
        }
        // Check if knight can move 2 rows up and 1 column left
        if (row < 6 && column > 0) {
            int move = position + 15;
            knightCaptures.push_back(move);
        }
        // Check if knight can move 2 rows down and 1 column right
        if (row > 1 && column < 7) {
            int move = position - 15;
            knightCaptures.push_back(move);
        }
        // Check if knight can move 2 rows down and 1 column left
        if (row > 1 && column > 0) {
            int move = position - 17;
            knightCaptures.push_back(move);
        }
        // Check if knight can move 1 row up and 2 columns right
        if (row < 7 && column < 6) {
            int move = position + 10;
            knightCaptures.push_back(move);
        }
        // Check if knight can move 1 row up and 2 columns left
        if (row < 7 && column > 1) {
            int move = position + 6;
            knightCaptures.push_back(move);
        }
        // Check if knight can move 1 row down and 2 columns right
        if (row > 0 && column < 6) {
            int move = position - 6;
            knightCaptures.push_back(move);
        }
        // Check if knight can move 1 row down and 2 columns left
        if (row > 0 && column > 1) {
            int move = position - 10;
            knightCaptures.push_back(move);
        }
        knightsBitboard &= knightsBitboard - 1ULL;
    }
    return knightCaptures;
}

std::vector<int> MoveGenerator::GenerateBishopCaptures() const {
    std::vector<int> bishopCaptures;

    uint64_t bishopsBitboard = board->bishopsBitboard;

    while (bishopsBitboard) {
        const int position = __builtin_ctzll(bishopsBitboard);
        if (board->getPieceColor(position) != board->turn) {
            bishopsBitboard &= bishopsBitboard - 1ULL;
            continue;
        }
        // Generate all moves, don't check if they are valid
        int row = position / 8;
        int column = position % 8;

        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column + i > 7) {
                break;
            }
            int move = position + (9 * i);

            if (board->getPieceType(move) == NONE) {
                bishopCaptures.push_back(move);
            } else {
                bishopCaptures.push_back(move);
                break;
            }
        }

        // Check if bishop can move up and left
        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column - i < 0) {
                break;
            }
            int move = position + (7 * i);

            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE) {
                bishopCaptures.push_back(move);
            } else {
                bishopCaptures.push_back(move);
                break;
            }
        }

        // Check if bishop can move down and right
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column + i > 7) {
                break;
            }
            int move = position - (7 * i);

            if (board->getPieceType(move) == NONE) {
                bishopCaptures.push_back(move);
            } else {
                bishopCaptures.push_back(move);
                break;
            }
        }

        // Check if bishop can move down and left
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column - i < 0) {
                break;
            }
            int move = position - (9 * i);

            if (board->getPieceType(move) == NONE) {
                bishopCaptures.push_back(move);
            } else {
                bishopCaptures.push_back(move);
                break;
            }
        }
        bishopsBitboard &= bishopsBitboard - 1ULL;
    }

    return bishopCaptures;
}

std::vector<int> MoveGenerator::GenerateRookCaptures() const {
    std::vector<int> rookCaptures;

    uint64_t rooksBitboard = board->rooksBitboard;

    while (rooksBitboard) {
        const int position = __builtin_ctzll(rooksBitboard);
        if (board->getPieceColor(position) != board->turn) {
            rooksBitboard &= rooksBitboard - 1ULL;
            continue;
        }
        // Generate all moves, don't check if they are valid
        int row = position / 8;
        int column = position % 8;

        for (int i = row + 1; i < 8; i++) {
            int move = position + (8 * (i - row));

            if (board->getPieceType(move) == NONE) {
                rookCaptures.push_back(move);
            } else  {
                rookCaptures.push_back(move);
                break;
            }
        }
        // Check if rook can move down
        for (int i = row - 1; i >= 0; i--) {
            int move = position - (8 * (row - i));

            if (board->getPieceType(move) == NONE) {
                rookCaptures.push_back(move);
            } else {
                rookCaptures.push_back(move);
                break;
            }
        }
        // Check if rook can move right
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);

            if (board->getPieceType(move) == NONE) {
                rookCaptures.push_back(move);
            } else {
                rookCaptures.push_back(move);
                break;
            }
        }
        // Check if rook can move left
        for (int i = column - 1; i >= 0; i--) {
            int move = position - (column - i);

            if (board->getPieceType(move) == NONE) {
                rookCaptures.push_back(move);
            } else if (board->getPieceColor(move) != board->turn) {
                rookCaptures.push_back(move);
                break;
            }
        }
        rooksBitboard &= rooksBitboard - 1ULL;
    }

    return rookCaptures;
}

std::vector<int> MoveGenerator::GenerateQueenCaptures() const {
    std::vector<int> queenCaptures;

    uint64_t queensBitboard = board->queensBitboard;

    while (queensBitboard) {
        const int position = __builtin_ctzll(queensBitboard);
        if (board->getPieceColor(position) != board->turn) {
            queensBitboard &= queensBitboard - 1ULL;
            continue;
        }
        // Generate all moves, don't check if they are valid
        int row = position / 8;
        int column = position % 8;

        // Check if queen can move up
        for (int i = row + 1; i < 8; i++) {
            int move = position + (8 * (i - row));
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE) {
                break;
            }
        }

        // Check if queen can move down
        for (int i = row - 1; i >= 0; i--) {
            int move = position - (8 * (row - i));
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE) {
                break;
            }
        }

        // Check if queen can move right
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE) {
                break;
            }
        }

        // Check if queen can move left
        for (int i = column - 1; i >= 0; i--) {
            int move = position - (column - i);
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE) {
                break;
            }
        }

        // Check if queen can move up and right
        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column + i > 7) {
                break;
            }
            int move = position + (9 * i);
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE) {
                break;
            }
        }

        // Check if queen can move up and left
        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column - i < 0) {
                break;
            }
            int move = position + (7 * i);
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE) {
                break;
            }
        }

        // Check if queen can move down and right
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column + i > 7) {
                break;
            }
            int move = position - (7 * i);
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE) {
                break;
            }
        }

        // Check if queen can move down and left
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column - i < 0) {
                break;
            }
            int move = position - (9 * i);
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE) {
            break;
            }
        }
        queensBitboard &= queensBitboard - 1ULL;
    }
    return queenCaptures;
}

std::vector<int> MoveGenerator::GenerateKingCaptures() const {
    std::vector<int> kingCaptures;

    uint64_t kingsBitboard = board->kingsBitboard;

    while (kingsBitboard) {
        const int position = __builtin_ctzll(kingsBitboard);
        if (board->getPieceColor(position) != board->turn) {
            kingsBitboard &= kingsBitboard - 1ULL;
            continue;
        }
        const int row = position / 8;
        const int column = position % 8;
        // Check if king can move 1 row up
        if (row < 7) {
            int move = position + 8;
            kingCaptures.push_back(move);
        }
        // Check if king can move 1 row down
        if (row > 0) {
            int move = position - 8;
            kingCaptures.push_back(move);
        }
        // Check if king can move 1 column right
        if (column < 7) {
            int move = position + 1;
            kingCaptures.push_back(move);
        }
        // Check if king can move 1 column left
        if (column > 0) {
            int move = position - 1;
            kingCaptures.push_back(move);
        }
        // Check if king can move 1 row up and 1 column right
        if (row < 7 && column < 7) {
            int move = position + 9;
            kingCaptures.push_back(move);
        }
        // Check if king can move 1 row up and 1 column left
        if (row < 7 && column > 0) {
            int move = position + 7;
            kingCaptures.push_back(move);
        }
        // Check if king can move 1 row down and 1 column right
        if (row > 0 && column < 7) {
            int move = position - 7;
            kingCaptures.push_back(move);
        }
        // Check if king can move 1 row down and 1 column left
        if (row > 0 && column > 0) {
            int move = position - 9;
            kingCaptures.push_back(move);
        }
        kingsBitboard &= kingsBitboard - 1ULL;
    }
    return kingCaptures;
}
