#include "MoveGenerator.h"
#include <iostream>
#include <chrono>

MoveGenerator::MoveGenerator(Board* board) {
    this->board = board;
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
    const std::vector<Piece*> pawns = board->pawns[board->turn];
    const int promotionRow = board->turn == "white" ? 7 : 0;
    const int direction = board->turn == "white" ? 1 : -1;
    const int startingRow = board->turn == "white" ? 1 : 6;
    const int enPassantRight = board->turn == "white" ? 9 : 7;
    const int enPassantLeft = board->turn == "white" ? 7 : 9;

    // Loop through all pawns
    for (Piece* piece : pawns) {
        if (piece == nullptr) {
            continue;
        }
        std::map<int, Move*> pawnMoves;
        int position = piece->position;
        int row = position / 8;
        int column = position % 8;

        // Check if pawn can move forward one square
        const int forwardOneSquare = position + (direction * 8);
        if (board->board[forwardOneSquare] == nullptr) {
            // Check if pawn can promote
            if (board->inCheck && board->movesThatBlockCheck.size() > 0) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), forwardOneSquare) != board->movesThatBlockCheck.end()) {
                    pawnMoves.emplace(std::pair{position, new Move(position, forwardOneSquare, board)});
                }
            } else {
                pawnMoves.emplace(std::pair{position, new Move(position, forwardOneSquare, board)});
            }
            if (row == startingRow) {
                int forwardTwoSquares = position + (direction * 16);
                if (board->board[forwardTwoSquares] == nullptr) {
                    if (board->inCheck && board->movesThatBlockCheck.size() > 0) {
                        if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), forwardTwoSquares) != board->movesThatBlockCheck.end()) {
                            pawnMoves.emplace(std::pair{position, new Move(position, forwardTwoSquares, board)});
                        }
                    } else {
                        pawnMoves.emplace(std::pair{position, new Move(position, forwardTwoSquares, board)});
                    }
                }
            }
        }
        if (piece->enPassantRight) {
            if (board->inCheck && board->movesThatBlockCheck.size() > 0) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantRight)) != board->movesThatBlockCheck.end()) {
                    pawnMoves.emplace(std::pair{position, new Move(position, position + (direction * enPassantRight), board)});
                }
            } else {
                pawnMoves.emplace(std::pair{position, (new Move(position, position + (direction * enPassantRight), board))});
            }
        }
        if (piece->enPassantLeft) {
            if (board->inCheck && board->movesThatBlockCheck.size() > 0) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), position + (direction * enPassantLeft)) != board->movesThatBlockCheck.end()) {
                    pawnMoves.emplace(std::pair{position, new Move(position, position + (direction * enPassantLeft), board)});
                }
            } else {
                pawnMoves.emplace(std::pair{position, new Move(position, position + (direction * enPassantLeft), board)});
            }
        }
        // check if we can capture a piece to the right
        if (column != 7) {
            int rightCapture = board->boardArray[row + 1 * direction][column + 1 * direction];
            if (board->board[rightCapture] != nullptr && board->board[rightCapture]->color != board->turn) {
                if (board->inCheck && board->movesThatBlockCheck.size() > 0) {
                    if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), rightCapture) != board->movesThatBlockCheck.end()) {
                        pawnMoves.emplace(std::pair{position, new Move(position, rightCapture, board)});
                    }
                } else {
                    pawnMoves.emplace(std::pair{position, new Move(position, rightCapture, board)});
                }
            }
        }
        // check if we can capture a piece to the left
        if (column != 0) {
            int leftCapture = board->boardArray[row + 1 * direction][column - 1 * direction];
            if (board->board[leftCapture] != nullptr && board->board[leftCapture]->color != board->turn) {
                if (board->inCheck && board->movesThatBlockCheck.size() > 0) {
                    if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), leftCapture) != board->movesThatBlockCheck.end()) {
                        pawnMoves.emplace(std::pair{position, new Move(position, leftCapture, board)});
                    }
                } else {
                    pawnMoves.emplace(std::pair{position, new Move(position, leftCapture, board)});
                }
            }
        }

        piece->moves = pawnMoves;
    }
}

void MoveGenerator::GenerateKnightMoves() {
    std::vector<Piece*> knights = board->knights[board->turn];

    for (Piece* piece: knights) {
        std::map<int, Move*> knightMoves;
        if (piece == nullptr) {
            continue;
        }
        const int position = piece->position;
        const int row = position / 8;
        const int column = position % 8;

        // Check if knight can move 2 rows up and 1 column right
        if (row < 6 && column < 7) {
            int move = position + 17;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if knight can move 2 rows up and 1 column left
        if (row < 6 && column > 0) {
            int move = position + 15;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if knight can move 2 rows down and 1 column right
        if (row > 1 && column < 7) {
            int move = position - 15;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if knight can move 2 rows down and 1 column left
        if (row > 1 && column > 0) {
            int move = position - 17;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if knight can move 1 row up and 2 columns right
        if (row < 7 && column < 6) {
            int move = position + 10;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if knight can move 1 row up and 2 columns left
        if (row < 7 && column > 1) {
            int move = position + 6;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if knight can move 1 row down and 2 columns right
        if (row > 0 && column < 6) {
            int move = position - 6;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if knight can move 1 row down and 2 columns left
        if (row > 0 && column > 1) {
            int move = position - 10;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                knightMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }

        if (board->inCheck) {
            std::map<int, Move*> validMoves;

            for (std::pair move : validMoves) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move.second->toSquare) == board->movesThatBlockCheck.end()) {
                    validMoves.emplace(move);
                }
            }
            knightMoves = validMoves;
        }

        piece->moves = knightMoves;
    }
}

void MoveGenerator::GenerateRookMoves() {
    const std::vector<Piece*> rooks = board->rooks[board->turn];

    for (Piece* piece: rooks) {
        const int position = piece->position;
        const int row = position / 8;
        const int column = position % 8;

        std::map<int, Move*> rookMoves;

        // Check if rook can move up
        for (int i = row + 1; i < 8; i++) {
            int move = position + (8 * (i - row));
            if (board->board[move] == nullptr) {
                rookMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                rookMoves.emplace(std::pair{position, new Move(position, move, board)});
                break;
            } else {
                break;
            }
        }
        // Check if rook can move down
        for (int i = row - 1; i >= 0; i--) {
            int move = position - (8 * (row - i));
            if (board->board[move] == nullptr) {
                rookMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                rookMoves.emplace(std::pair{position, new Move(position, move, board)});
                break;
            } else {
                break;
            }
        }
        // Check if rook can move right
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);
            if (board->board[move] == nullptr) {
                rookMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                rookMoves.emplace(std::pair{position, new Move(position, move, board)});
                break;
            } else {
                break;
            }
        }
        // Check if rook can move left
        for (int i = column - 1; i >= 0; i--) {
            int move = position - (column - i);
            if (board->board[move] == nullptr) {
                rookMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                rookMoves.emplace(std::pair{position, new Move(position, move, board)});
                break;
            } else {
                break;
            }
        }

        if (board->inCheck) {
            std::map<int, Move*> validMoves;

            for (std::pair move : validMoves ) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move.second->toSquare) != board->movesThatBlockCheck.end()) {
                    validMoves.emplace(move);
                }
            }
            rookMoves = validMoves;
        }

        piece->moves = rookMoves;

    }
}

void MoveGenerator::GenerateBishopMoves() {
    const std::vector<Piece*> bishops = board->bishops[board->turn];

    for (Piece* piece: bishops) {
        const int position = piece->position;
        const int row = position / 8;
        const int column = position % 8;

        std::map<int, Move*> bishopMoves;
        if (piece == nullptr) {
            continue;
        }

        // Check if bishop can move up and right
        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column + i > 7) {
                break;
            }
            int move = position + (9 * i);
            if (board->board[move] == nullptr) {
                bishopMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                bishopMoves.emplace(std::pair{position, new Move(position, move, board)});
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
            if (board->board[move] == nullptr) {
                bishopMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                bishopMoves.emplace(std::pair{position, new Move(position, move, board)});
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
            if (board->board[move] == nullptr) {
                bishopMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                bishopMoves.emplace(std::pair{position, new Move(position, move, board)});
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
            if (board->board[move] == nullptr) {
                bishopMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                bishopMoves.emplace(std::pair{position, new Move(position, move, board)});
                break;
            } else {
                break;
            }
        }

        if (board->inCheck) {
            std::map<int, Move*> validMoves;

            for (std::pair move: validMoves ) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move.second->toSquare) != board->movesThatBlockCheck.end()) {
                    validMoves.emplace(move);
                }
            }
            bishopMoves = validMoves;
        }

        piece->moves = bishopMoves;
    }
}

void MoveGenerator::GenerateQueenMoves() {
    const std::vector<Piece*> queens = board->queens[board->turn];

    for (Piece* piece: queens) {
        const int position = piece->position;
        const int row = position / 8;
        const int column = position % 8;

        std::map<int, Move*> queenMoves;
        if (piece == nullptr) {
            continue;
        }

        // Check if queen can move up
        for (int i = row + 1; i < 8; i++) {
            int move = position + (8 * (i - row));
            if (board->board[move] == nullptr) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
                break;
            } else {
                break;
            }
        }

        // Check if queen can move down
        for (int i = row - 1; i >= 0; i--) {
            int move = position - (8 * (row - i));
            if (board->board[move] == nullptr) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
                break;
            } else {
                break;
            }
        }

        // Check if queen can move right
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);
            if (board->board[move] == nullptr) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
                break;
            } else {
                break;
            }
        }

        // Check if queen can move left
        for (int i = column - 1; i >= 0; i--) {
            int move = position - (column - i);
            if (board->board[move] == nullptr) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
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
            if (board->board[move] == nullptr) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
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
            if (board->board[move] == nullptr) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
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
            if (board->board[move] == nullptr) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
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
            if (board->board[move] == nullptr) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
            } else if (board->board[move]->color != board->turn) {
                queenMoves.emplace(std::pair{position, new Move(position, move, board)});
                break;
            } else {
                break;
            }
        }

        if (board->inCheck) {
            std::map<int, Move*> validMoves;

            for (std::pair move : validMoves ) {
                if (std::find(board->movesThatBlockCheck.begin(), board->movesThatBlockCheck.end(), move.second->toSquare) != board->movesThatBlockCheck.end()) {
                    validMoves.emplace(move);
                }
            }
            queenMoves = validMoves;
        }

        piece->moves = queenMoves;
    }
}

void MoveGenerator::GenerateKingMoves() {
    const std::vector<Piece*> kings = board->kings[board->turn];

    for (Piece* piece: kings) {
        std::map<int, Move*> kingMoves;
        if (piece == nullptr) {
            continue;
        }
        const int position = piece->position;
        const int row = position / 8;
        const int column = position % 8;

        // Check if king can move 1 row up
        if (row < 7) {
            int move = position + 8;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                kingMoves.emplace(std::pair{position, new Move(position, move, board)});

            }
        }
        // Check if king can move 1 row down
        if (row > 0) {
            int move = position - 8;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                kingMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if king can move 1 column right
        if (column < 7) {
            int move = position + 1;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                kingMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if king can move 1 column left
        if (column > 0) {
            int move = position - 1;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                kingMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if king can move 1 row up and 1 column right
        if (row < 7 && column < 7) {
            int move = position + 9;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                kingMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if king can move 1 row up and 1 column left
        if (row < 7 && column > 0) {
            int move = position + 7;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                kingMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if king can move 1 row down and 1 column right
        if (row > 0 && column < 7) {
            int move = position - 7;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                kingMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }
        // Check if king can move 1 row down and 1 column left
        if (row > 0 && column > 0) {
            int move = position - 9;
            if (board->board[move] == nullptr || board->board[move]->color != board->turn) {
                kingMoves.emplace(std::pair{position, new Move(position, move, board)});
            }
        }

        std::map<int, Move*> validMoves;
        for (auto move : kingMoves) {

            if (std::find(board->controlledSquares.begin(), board->controlledSquares.end(), move.second->toSquare) == board->controlledSquares.end()) {
                validMoves.emplace(move);
            }
        }
        kingMoves = validMoves;

        piece->moves = kingMoves;
    }
}
