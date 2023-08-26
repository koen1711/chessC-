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
    const std::vector<int> pawns = board->pawns[board->turn];
    const bool blackTurn = board->turn == ChessColor::COLORBLACK;
    const int promotionRow = blackTurn ? 7 : 0;
    const int direction = blackTurn ? 1 : -1;
    const int startingRow = blackTurn ? 1 : 6;
    const int enPassantRight = blackTurn ? 9 : 7;
    const int enPassantLeft = blackTurn ? 7 : 9;

    // Loop through all pawns
    for (auto piece : pawns) {
        Piece* p = board->pieces[piece];
        GenerateOnePawnMoves(p, direction, startingRow, enPassantRight, enPassantLeft);
    }
}

void MoveGenerator::GenerateOnePawnMoves(Piece* piece, int direction, int startingRow, int enPassantRight, int enPassantLeft) const {
    std::vector<Move*> pawnMoves;
    int position = piece->position;
    int row = position / 8;
    int column = position % 8;

    // Check if pawn can move forward one square
    const int forwardOneSquare = boardArray[row + 1 * direction][column];
    if (board->board[forwardOneSquare] == nullptr) {
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
            if (board->board[forwardTwoSquares] == nullptr) {
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
    if (piece->pFlags->ENPASSANTRIGHT) {
        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantRight)) != board->movesThatBlockCheck.end()) {
                pawnMoves.push_back(new Move(position, position + (direction * enPassantRight), board));
            }
        } else {
            pawnMoves.push_back((new Move(position, position + (direction * enPassantRight), board)));
        }
    }
    if (piece->pFlags->ENPASSANTLEFT) {
        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantLeft)) != board->movesThatBlockCheck.end()) {
                pawnMoves.push_back(new Move(position, position + (direction * enPassantLeft), board));
            }
        } else {
            pawnMoves.push_back(new Move(position, position + (direction * enPassantLeft), board));
        }
    }

    // check if we can capture a piece to the right
    if ((column != 7 && piece->color == ChessColor::COLORBLACK) || (column != 0 && piece->color == ChessColor::COLORWHITE)) {
        int rightCapture = boardArray[row + 1 * direction][column + 1 * direction];
        if (board->board[rightCapture] != nullptr && board->board[rightCapture]->color != board->turn) {
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
    if ((column != 0 && piece->color == ChessColor::COLORBLACK) || (column != 7 && piece->color == ChessColor::COLORWHITE)) {
        int leftCapture = boardArray[row + 1 * direction][column - 1 * direction];
        if (board->board[leftCapture] != nullptr && board->board[leftCapture]->color != board->turn) {
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), leftCapture) != board->movesThatBlockCheck.end()) {
                    pawnMoves.push_back(new Move(position, leftCapture, board));
                }
            } else {
                pawnMoves.push_back(new Move(position, leftCapture, board));
            }
        }
    }

    if (piece->pFlags->ENPASSANTLEFT) {
        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantLeft)) != board->movesThatBlockCheck.end()) {
                pawnMoves.push_back(new Move(position, position + (direction * enPassantLeft), board));
            }
        } else {
            pawnMoves.push_back((new Move(position, position + (direction * enPassantLeft), board)));
        }
    } else if (piece->pFlags->ENPASSANTRIGHT) {
        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantRight)) != board->movesThatBlockCheck.end()) {
                pawnMoves.push_back(new Move(position, position + (direction * enPassantRight), board));
            }
        } else {
            pawnMoves.push_back((new Move(position, position + (direction * enPassantRight), board)));
        }
    }

    piece->moves = pawnMoves;
}

void MoveGenerator::GenerateKnightMoves() {
    std::vector<int> knights = board->knights[board->turn];

    for (auto piece: knights) {
        Piece* p = board->pieces[piece];
        GenerateOneKnightMoves(p);
    }
}

void MoveGenerator::GenerateOneKnightMoves(Piece* piece) const {
    std::vector<Move*> knightMoves = {};

    const int position = piece->position;
    const int row = position / 8;
    const int column = position % 8;

    // Check if knight can move 2 rows up and 1 column right
    if (row < 6) {
        if (column < 7) {
            int move = position + 17;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
        // Check if knight can move 2 rows up and 1 column left
        if (column > 0) {
            int move = position + 15;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
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
                    if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
        // Check if knight can move 2 rows down and 1 column left
        if (column > 0) {
            int move = position - 17;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
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
                    if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
        // Check if knight can move 1 row up and 2 columns left
        if (column > 1) {
            int move = position + 6;
            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
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
                    if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
        // Check if knight can move 1 row down and 2 columns left
        if (column > 1) {
            int move = position - 10;

            if (board->inCheck) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) !=
                    board->movesThatBlockCheck.end()) {
                    if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                        knightMoves.push_back(new Move(position, move, board));
                    }
                }
            } else if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.push_back(new Move(position, move, board));
            }
        }
    }
    piece->moves = knightMoves;
}

void MoveGenerator::GenerateRookMoves() {
    const std::vector<int> rooks = board->rooks[board->turn];

    for (auto piece: rooks) {
        Piece* p = board->pieces[piece];
        GenerateOneRookMoves(p);
    }
}

void MoveGenerator::GenerateOneRookMoves( Piece* piece) const {
    const int position = piece->position;
    const int row = position / 8;
    const int column = position % 8;

    std::vector<Move*> rookMoves;

    // Check if rook can move up
    for (int i = row + 1; i < 8; i++) {
        int move = position + (8 * (i - row));

        if (board->board[move] != nullptr && board->board[move]->color == board->turn) {
            break;
        }

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->board[move] == nullptr) {
            rookMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
            rookMoves.push_back(new Move(position, move, board));
            break;
        }
  }
    // Check if rook can move down
    for (int i = row - 1; i >= 0; i--) {
        int move = position - (8 * (row - i));

        if (board->board[move] != nullptr && board->board[move]->color == board->turn) {
            break;
        }

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->board[move] == nullptr) {
            rookMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
            rookMoves.push_back(new Move(position, move, board));
            break;
        }
    }
    // Check if rook can move right
    for (int i = column + 1; i < 8; i++) {
        int move = position + (i - column);

        if (board->board[move] != nullptr && board->board[move]->color == board->turn) {
            break;
        }

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->board[move] == nullptr) {
            rookMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
            rookMoves.push_back(new Move(position, move, board));
            break;
        }
    }
    // Check if rook can move left
    for (int i = column - 1; i >= 0; i--) {
        int move = position - (column - i);

        if (board->board[move] != nullptr && board->board[move]->color == board->turn) {
            break;
        }

        if (board->inCheck) {
            if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move) == board->movesThatBlockCheck.end()) {
                continue;
            }
        }

        if (board->board[move] == nullptr) {
            rookMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
            rookMoves.push_back(new Move(position, move, board));
            break;
        }
    }

    piece->moves = rookMoves;
}

void MoveGenerator::GenerateBishopMoves() {
    const std::vector<int> bishops = board->bishops[board->turn];

    for (auto piece: bishops) {
        Piece* p = board->pieces[piece];
        GenerateOneBishopMoves(p);
    }
}

void MoveGenerator::GenerateOneBishopMoves( Piece* piece) const {
    const int position = piece->position;
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

        if (board->board[move] == nullptr) {
            bishopMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            bishopMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            bishopMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            bishopMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
            bishopMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    piece->moves = bishopMoves;
}

void MoveGenerator::GenerateQueenMoves() {
    const std::vector<int> queens = board->queens[board->turn];

    for (auto piece: queens) {
        Piece* p = board->pieces[piece];
        GenerateOneQueenMoves(p);
    }
}

void MoveGenerator::GenerateOneQueenMoves( Piece* piece) const {
    const int position = piece->position;
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

        if (board->board[move] == nullptr) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
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

        if (board->board[move] == nullptr) {
            queenMoves.push_back(new Move(position, move, board));
        } else if (board->board[move]->color != board->turn) {
            queenMoves.push_back(new Move(position, move, board));
            break;
        } else {
            break;
        }
    }

    piece->moves = queenMoves;
}

void MoveGenerator::GenerateKingMoves() {
    Piece* king = board->pieces[board->kings.at(board->turn)];
    GenerateOneKingMoves(king);
}

void MoveGenerator::GenerateOneKingMoves( Piece* piece) const {
    std::vector<Move*> kingMoves;
    const int position = piece->position;
    const int row = position / 8;
    const int column = position % 8;

    // Check if king can move 1 row up
    if (row < 7) {
        int move = position + 8;
        if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
            kingMoves.push_back(new Move(position, move, board));

        }
    }
    // Check if king can move 1 row down
    if (row > 0) {
        int move = position - 8;
        if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 column right
    if (column < 7) {
        int move = position + 1;
        if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 column left
    if (column > 0) {
        int move = position - 1;
        if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row up and 1 column right
    if (row < 7 && column < 7) {
        int move = position + 9;
        if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row up and 1 column left
    if (row < 7 && column > 0) {
        int move = position + 7;
        if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row down and 1 column right
    if (row > 0 && column < 7) {
        int move = position - 7;
        if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row down and 1 column left
    if (row > 0 && column > 0) {
        int move = position - 9;
        if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }

    if (!board->inCheck) {
        // Check if king can castle
        if (board->turn == ChessColor::COLORWHITE) {
            // check if there is a rook on the king side
            if (board->board[63] != nullptr && board->board[63]->name == PieceType::ROOK && board->board[63]->color == ChessColor::COLORWHITE &&
                !board->board[63]->pFlags->MOVED) {
                // check if there are no pieces between the king and the rook
                if (board->board[61] == nullptr && board->board[62] == nullptr && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 61) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 62) == board->controlledSquares.end()) {
                    kingMoves.push_back(new Move(position, 62, board));
                }
            }
            // check if there is a rook on the queen side
            if (board->board[56] != nullptr && board->board[56]->name == PieceType::ROOK && board->board[56]->color == ChessColor::COLORWHITE &&
                !board->board[56]->pFlags->MOVED) {
                // check if there are no pieces between the king and the rook
                if (board->board[57] == nullptr && board->board[58] == nullptr && board->board[59] == nullptr && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 57) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 58) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 59) == board->controlledSquares.end()) {
                    kingMoves.push_back(new Move(position, 58, board));
                }
            }

        } else {
            // check if there is a rook on the king side
            if (board->board[7] != nullptr && board->board[7]->name == PieceType::ROOK && board->board[7]->color == ChessColor::COLORBLACK &&
                !board->board[7]->pFlags->MOVED) {
                // check if there are no pieces between the king and the rook
                if (board->board[5] == nullptr && board->board[6] == nullptr && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 5) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 6) == board->controlledSquares.end()) {
                    kingMoves.push_back(new Move(position, 6, board));
                }
            }
            // check if there is a rook on the queen side
            if (board->board[0] != nullptr && board->board[0]->name == PieceType::ROOK && board->board[0]->color == ChessColor::COLORBLACK &&
                !board->board[0]->pFlags->MOVED) {
                // check if there are no pieces between the king and the rook
                if (board->board[1] == nullptr && board->board[2] == nullptr && board->board[3] == nullptr && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 1) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 2) == board->controlledSquares.end() && std::find(board->controlledSquares.begin(), board->controlledSquares.end(), 3) == board->controlledSquares.end()) {
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

    piece->moves = kingMoves;
}



std::vector<int> MoveGenerator::GeneratePawnCaptures() const {
    std::vector<int> pawnCaptures;

    for (auto pos: board->pawns[board->turn]) {
        Piece* piece = board->pieces[pos];
        const int row = piece->position / 8;
        const int column = piece->position % 8;
        if (piece->color == ChessColor::COLORWHITE) {
            if (column > 0) {
                pawnCaptures.push_back(piece->position - 7);
            }
            if (column < 7) {
                pawnCaptures.push_back(piece->position - 9);
            }
        } else {
            if (column > 0) {
                pawnCaptures.push_back(piece->position + 9);
            }
            if (column < 7) {
                pawnCaptures.push_back(piece->position + 7);
            }
        }
    }
    return pawnCaptures;
}

std::vector<int> MoveGenerator::GenerateKnightCaptures() const {
    std::vector<int> knightCaptures;

    for (auto pos : board->knights[board->turn]) {
        // Generate all moves, don't check if they are vali
        const int position = board->pieces[pos]->position;
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
    }
    return knightCaptures;
}

std::vector<int> MoveGenerator::GenerateBishopCaptures() const {
    std::vector<int> bishopCaptures;

    for (auto pos: board->bishops[board->turn]) {
        // Generate all moves, don't check if they are valid
        const int position = board->pieces[pos]->position;
        int row = position / 8;
        int column = position % 8;

        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column + i > 7) {
                break;
            }
            int move = position + (9 * i);

            if (board->board[move] == nullptr) {
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

            if (board->board[move] == nullptr) {
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

            if (board->board[move] == nullptr) {
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

            if (board->board[move] == nullptr) {
                bishopCaptures.push_back(move);
            } else {
                bishopCaptures.push_back(move);
                break;
            }
        }
    }

    return bishopCaptures;
}

std::vector<int> MoveGenerator::GenerateRookCaptures() const {
    std::vector<int> rookCaptures;

    for (auto pos : board->rooks[board->turn]) {
        // Generate all moves, don't check if they are valid
        const int position = board->pieces[pos]->position;
        int row = position / 8;
        int column = position % 8;

        for (int i = row + 1; i < 8; i++) {
            int move = position + (8 * (i - row));

            if (board->board[move] == nullptr) {
                rookCaptures.push_back(move);
            } else {
                rookCaptures.push_back(move);
                break;
            }
        }
        // Check if rook can move down
        for (int i = row - 1; i >= 0; i--) {
            int move = position - (8 * (row - i));

            if (board->board[move] == nullptr) {
                rookCaptures.push_back(move);
            } else {
                rookCaptures.push_back(move);
                break;
            }
        }
        // Check if rook can move right
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);

            if (board->board[move] == nullptr) {
                rookCaptures.push_back(move);
            } else {
                rookCaptures.push_back(move);
                break;
            }
        }
        // Check if rook can move left
        for (int i = column - 1; i >= 0; i--) {
            int move = position - (column - i);

            if (board->board[move] == nullptr) {
                rookCaptures.push_back(move);
            } else if (board->board[move]->color != board->turn) {
                rookCaptures.push_back(move);
                break;
            }
        }
    }

    return rookCaptures;
}

std::vector<int> MoveGenerator::GenerateQueenCaptures() const {
    std::vector<int> queenCaptures;

    for (auto pos : board->queens[board->turn]) {
        // Generate all moves, don't check if they are valid
        const int position = board->pieces[pos]->position;
        int row = position / 8;
        int column = position % 8;

        // Check if queen can move up
        for (int i = row + 1; i < 8; i++) {
            int move = position + (8 * (i - row));
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->board[move] != nullptr) {
                break;
            }
        }

        // Check if queen can move down
        for (int i = row - 1; i >= 0; i--) {
            int move = position - (8 * (row - i));
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->board[move] != nullptr) {
                break;
            }
        }

        // Check if queen can move right
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->board[move] != nullptr) {
                break;
            }
        }

        // Check if queen can move left
        for (int i = column - 1; i >= 0; i--) {
            int move = position - (column - i);
            queenCaptures.push_back(move);
            // If there is a piece on the square, stop checking this direction
            if (board->board[move] != nullptr) {
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
            if (board->board[move] != nullptr) {
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
            if (board->board[move] != nullptr) {
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
            if (board->board[move] != nullptr) {
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
            if (board->board[move] != nullptr) {
            break;
            }
        }


    }
    return queenCaptures;
}

std::vector<int> MoveGenerator::GenerateKingCaptures() const {
    std::vector<int> kingCaptures;
    const int position = board->pieces[board->kings.at(board->turn)]->position;
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

    return kingCaptures;
}
