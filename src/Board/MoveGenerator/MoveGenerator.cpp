#include "MoveGenerator.h"
#include <thread>

MoveGenerator::MoveGenerator(Board* board) : board(board) {};


void MoveGenerator::GenerateCapturesOnly() {
    board->controlledSquaresBitboard = 0ULL;
    board->controlledSquaresBitboard |= GeneratePawnCaptures();
    board->controlledSquaresBitboard |= GenerateKnightCaptures();
    board->controlledSquaresBitboard |= GenerateBishopCaptures();
    board->controlledSquaresBitboard |= GenerateRookCaptures();
    board->controlledSquaresBitboard |= GenerateQueenCaptures();
    board->controlledSquaresBitboard |= GenerateKingCaptures();
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
    //const int promotionRow = blackTurn ? 7 : 0;
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
    std::vector<Move*> pawnMoves = {};
    pawnMoves.reserve(4);
    ChessColor enemyColor = board->turn == ChessColor::COLORWHITE ? ChessColor::COLORBLACK : ChessColor::COLORWHITE;
    int row = position / 8;
    int column = position % 8;
    Directions pinDirection = board->getPinDirection(position);
    if (pinDirection == Directions::HORIZONTALLEFT || pinDirection == Directions::HORIZONTALRIGHT) {
        return;
    }

    // Check if pawn can move forward one square
    const int forwardOneSquare = position + (direction * 8);
    if (board->getPieceColor(forwardOneSquare) == ChessColor::COLORNONE && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::VERTICALUP || pinDirection == Directions::VERTICALDOWN )) ) {
        // Check if pawn can promote
        if (board->inCheck) {
            if (board->movesThatBlockCheckBitboard & (1ULL << forwardOneSquare)) {
                pawnMoves.push_back(new Move(position, forwardOneSquare, board));
            }
        } else {
            pawnMoves.push_back(new Move(position, forwardOneSquare, board));
        }
        if (row == startingRow) {
            int forwardTwoSquares = position + (direction * 16);
            if (board->getPieceColor(forwardTwoSquares) == ChessColor::COLORNONE) {
                if (board->inCheck) {
                    if (board->movesThatBlockCheckBitboard & (1ULL << forwardTwoSquares)) {
                        pawnMoves.push_back(new Move(position, forwardTwoSquares, board));
                    }
                } else {
                    pawnMoves.push_back(new Move(position, forwardTwoSquares, board));
                }
            }
        }
    }
    if (board->enPassantRightBitboard & (1ULL << position)) {
        if (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPRIGHT || pinDirection == Directions::DIAGONALDOWNRIGHT)) {
            if (board->inCheck) {
                if (board->movesThatBlockCheckBitboard & (1ULL << (position + (direction * enPassantRight)))) {
                    pawnMoves.push_back(new Move(position, position + (direction * enPassantRight), board));
                }
            } else {
                pawnMoves.push_back((new Move(position, position + (direction * enPassantRight), board)));
            }
        }
    }
    if (board->enPassantLeftBitboard & (1ULL << position)) {
        if (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPLEFT || pinDirection == Directions::DIAGONALDOWNLEFT)) {
            if (board->inCheck) {
                if (board->movesThatBlockCheckBitboard & (1ULL << (position + (direction * enPassantLeft)))) {
                    pawnMoves.push_back(new Move(position, position + (direction * enPassantLeft), board));
                }
            } else {
                pawnMoves.push_back(new Move(position, position + (direction * enPassantLeft), board));
            }
        }
    }

    // check if we can capture a piece to the right
    if ((column != 0 && enemyColor == ChessColor::COLORWHITE || column != 7 && enemyColor == ChessColor::COLORBLACK) && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPRIGHT || pinDirection == Directions::DIAGONALDOWNRIGHT))) {
        int rightCapture = position + (direction * 7);
        if (board->getPieceColor(rightCapture) == enemyColor) {
            if (board->inCheck) {
                if (board->movesThatBlockCheckBitboard & (1ULL << rightCapture)) {
                    pawnMoves.push_back(new Move(position, rightCapture, board));
                }
            } else {
                pawnMoves.push_back(new Move(position, rightCapture, board));
            }
        }
    }
    // check if we can capture a piece to the left
    if ((column != 0 && enemyColor == ChessColor::COLORBLACK || column != 7 && enemyColor == ChessColor::COLORWHITE) && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPLEFT || pinDirection == Directions::DIAGONALDOWNLEFT))) {
        int leftCapture = position + (direction * 9);
        if (board->getPieceColor(leftCapture) == enemyColor) {
            if (board->inCheck) {
                if (board->movesThatBlockCheckBitboard & (1ULL << leftCapture)) {
                    pawnMoves.push_back(new Move(position, leftCapture, board));
                }
            } else {
                pawnMoves.push_back(new Move(position, leftCapture, board));
            }
        }
    }

    if (board->enPassantLeftBitboard & (1ULL << position)) {
        if (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPLEFT || pinDirection == Directions::DIAGONALDOWNLEFT)) {
            if (board->inCheck) {
                if (board->movesThatBlockCheckBitboard & (1ULL << (position + (direction * enPassantLeft)))) {
                    pawnMoves.push_back(new Move(position, position + (direction * enPassantLeft), board));
                }
            } else {
                pawnMoves.push_back((new Move(position, position + (direction * enPassantLeft), board)));
            }
        }
    } else if (board->enPassantRightBitboard & (1ULL << position)) {
        if (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPRIGHT || pinDirection == Directions::DIAGONALDOWNRIGHT)) {
            if (board->inCheck) {
                if (board->movesThatBlockCheckBitboard & (1ULL << (position + (direction * enPassantRight)))) {
                    pawnMoves.push_back(new Move(position, position + (direction * enPassantRight), board));
                }
            } else {
                pawnMoves.push_back((new Move(position, position + (direction * enPassantRight), board)));
            }
        }
    }

    board->moveMap[position] = pawnMoves;
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
    knightMoves.reserve(8);

    ChessColor color = board->getPieceColor(position);
    Directions pinDirection = board->getPinDirection(position);
    const int row = position / 8;
    const int column = position % 8;

    // Check if knight can move 2 rows up and 1 column right
    if (pinDirection == Directions::NODIRECTION) {
        if (row < 6) {
            if (column < 7) {
                int move = position + 17;
                if (board->inCheck) {
                    if (board->movesThatBlockCheckBitboard & (1ULL << move)) {
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
                    if (board->movesThatBlockCheckBitboard & (1ULL << move)) {
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
                    if (board->movesThatBlockCheckBitboard & (1ULL << move)) {
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
                    if (board->movesThatBlockCheckBitboard & (1ULL << move)) {
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
                    if (board->movesThatBlockCheckBitboard & (1ULL << move)) {
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
                    if (board->movesThatBlockCheckBitboard & (1ULL << move)) {
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
                    if (board->movesThatBlockCheckBitboard & (1ULL << move)) {
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
                    if (board->movesThatBlockCheckBitboard & (1ULL << move)) {
                        if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                            knightMoves.push_back(new Move(position, move, board));
                        }
                    }
                } else if (board->getPieceType(move) == NONE || board->getPieceColor(move) != color) {
                    knightMoves.push_back(new Move(position, move, board));
                }
            }
        }
    }
    board->moveMap[position] = knightMoves;
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
    Directions pinDirection = board->getPinDirection(position);

    std::vector<Move*> rookMoves = {};
    rookMoves.reserve(14);

    // Check if rook can move up
    if (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::VERTICALUP || pinDirection == Directions::VERTICALDOWN)) {
        for (int i = row + 1; i < 8; i++) {
            int move = position + (8 * (i - row));

            if (board->getPieceType(move) != NONE && board->getPieceColor(move) == color) {
                break;
            }

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
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
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
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
    }

    // Check if rook can move right
    if (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::HORIZONTALLEFT || pinDirection == Directions::HORIZONTALRIGHT)) {
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);

            if (board->getPieceType(move) != NONE && board->getPieceColor(move) == color) {
                break;
            }

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
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
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
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
    }

    board->moveMap[position] = rookMoves;
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

void MoveGenerator::GenerateOneBishopMoves(int position) const {
    const int row = position / 8;
    const int column = position % 8;

    ChessColor color = board->getPieceColor(position);
    std::vector<Move*> bishopMoves = {};
    bishopMoves.reserve(13);

    // Check if bishop can move up and right
    for (int i = 1; i < 8; i++) {
        if (row + i > 7 || column + i > 7) {
            break;
        }
        int move = position + (9 * i);

        if (board->getPieceColor(move) == color) {
            break;
        }

        if (board->inCheck) {
            if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            bishopMoves.push_back(new Move(position, move, board));
        } else {
            bishopMoves.push_back(new Move(position, move, board));
            break;
        }
    }

    // Check if bishop can move up and left
    for (int i = 1; i < 8; i++) {
        if (row + i > 7 || column - i < 0) {
            break;
        }

        int move = position + (7 * i);

        if (board->getPieceColor(move) == color) {
            break;
        }

        if (board->inCheck) {
            if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            bishopMoves.push_back(new Move(position, move, board));
        } else {
            bishopMoves.push_back(new Move(position, move, board));
            break;
        }
    }

    // Check if bishop can move down and right
    for (int i = 1; i < 8; i++) {
        if (row - i < 0 || column + i > 7) {
            break;
        }
        int move = position - (7 * i);

        if (board->getPieceColor(move) == color) {
            break;
        }

        if (board->inCheck) {
            if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            bishopMoves.push_back(new Move(position, move, board));
        } else {
            bishopMoves.push_back(new Move(position, move, board));
            break;
        }
    }

    // Check if bishop can move down and left
    for (int i = 1; i < 8; i++) {
        if (row - i < 0 || column - i < 0) {
            break;
        }
        int move = position - (9 * i);

        if (board->getPieceColor(move) == color) {
            break;
        }

        if (board->inCheck) {
            if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                continue;
            }
        }

        if (board->getPieceType(move) == NONE) {
            bishopMoves.push_back(new Move(position, move, board));
        } else {
            bishopMoves.push_back(new Move(position, move, board));
            break;
        }
    }

    board->moveMap[position] = bishopMoves;
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

    ChessColor color = board->getPieceColor(position);
    Directions pinDirection = board->getPinDirection(position);
    std::vector<Move*> queenMoves = {};
    queenMoves.reserve(28);

    // Check if queen can move up
    if (row < 7 && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::VERTICALUP || pinDirection == Directions::VERTICALDOWN))) {
        for (int i = row + 1; i < 8; i++) {
            int move = position + (8 * (i - row));

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE) {
                queenMoves.push_back(new Move(position, move, board));
            } else if (board->getPieceColor(move) != color) {
                queenMoves.push_back(new Move(position, move, board));
                break;
            } else {
                break;
            }
        }
    }

    // Check if queen can move down
    if (row > 0 && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::VERTICALUP || pinDirection == Directions::VERTICALDOWN))) {
        for (int i = row - 1; i >= 0; i--) {
            int move = position - (8 * (row - i));

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE) {
                queenMoves.push_back(new Move(position, move, board));
            } else if (board->getPieceColor(move) != color) {
                queenMoves.push_back(new Move(position, move, board));
                break;
            } else {
                break;
            }
        }
    }

    // Check if queen can move right
    if (column < 7 && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::HORIZONTALLEFT || pinDirection == Directions::HORIZONTALRIGHT))) {
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE) {
                queenMoves.push_back(new Move(position, move, board));
            } else if (board->getPieceColor(move) != color) {
                queenMoves.push_back(new Move(position, move, board));
                break;
            } else {
                break;
            }
        }
    }

    // Check if queen can move left
    if (column > 0 && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::HORIZONTALLEFT || pinDirection == Directions::HORIZONTALRIGHT))) {
        for (int i = column - 1; i >= 0; i--) {
            int move = position - (column - i);

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE) {
                queenMoves.push_back(new Move(position, move, board));
            } else if (board->getPieceColor(move) != color) {
                queenMoves.push_back(new Move(position, move, board));
                break;
            } else {
                break;
            }
        }
    }

        // Check if queen can move up and right
    if (row < 7 && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPRIGHT || pinDirection == Directions::DIAGONALDOWNRIGHT))) {
        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column + i > 7) {
                break;
            }
            int move = position + (9 * i);

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE) {
                queenMoves.push_back(new Move(position, move, board));
            } else if (board->getPieceColor(move) != color) {
                queenMoves.push_back(new Move(position, move, board));
                break;
            } else {
                break;
            }
        }
    }

        // Check if queen can move up and left
    if (row < 7 && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPLEFT || pinDirection == Directions::DIAGONALDOWNLEFT))) {
        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column - i < 0) {
                break;
            }
            int move = position + (7 * i);

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE) {
                queenMoves.push_back(new Move(position, move, board));
            } else if (board->getPieceColor(move) != color) {
                queenMoves.push_back(new Move(position, move, board));
                break;
            } else {
                break;
            }
        }
    }

    // Check if queen can move down and right
    if (row > 0 && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPRIGHT || pinDirection == Directions::DIAGONALDOWNRIGHT))) {
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column + i > 7) {
                break;
            }
            int move = position - (7 * i);

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE) {
                queenMoves.push_back(new Move(position, move, board));
            } else if (board->getPieceColor(move) != color) {
                queenMoves.push_back(new Move(position, move, board));
                break;
            } else {
                break;
            }
        }
    }

    // Check if queen can move down and left
    if (row > 0 && (pinDirection == Directions::NODIRECTION || (pinDirection == Directions::DIAGONALUPLEFT || pinDirection == Directions::DIAGONALDOWNLEFT))) {
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column - i < 0) {
                break;
            }
            int move = position - (9 * i);

            if (board->inCheck) {
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE) {
                queenMoves.push_back(new Move(position, move, board));
            } else if (board->getPieceColor(move) != color) {
                queenMoves.push_back(new Move(position, move, board));
                break;
            } else {
                break;
            }
        }
    }

    board->moveMap[position] = queenMoves;
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
    std::vector<Move*> kingMoves = {};
    kingMoves.reserve(8);
    const int row = position / 8;
    const int column = position % 8;
    ChessColor enemyColor = board->turn == ChessColor::COLORWHITE ? ChessColor::COLORBLACK : ChessColor::COLORWHITE;

    // Check if king can move 1 row up
    if (row < 7) {
        int move = position + 8;
        if ((board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) && (board->controlledSquaresBitboard & (1ULL << move)) == 0) {
            kingMoves.push_back(new Move(position, move, board));

        }
    }
    // Check if king can move 1 row down
    if (row > 0) {
        int move = position - 8;
        if ((board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) && (board->controlledSquaresBitboard & (1ULL << move)) == 0) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 column right
    if (column < 7) {
        int move = position + 1;
        if ((board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) && (board->controlledSquaresBitboard & (1ULL << move)) == 0) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 column left
    if (column > 0) {
        int move = position - 1;
        if ((board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) && (board->controlledSquaresBitboard & (1ULL << move)) == 0) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row up and 1 column right
    if (row < 7 && column < 7) {
        int move = position + 9;
        if ((board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) && (board->controlledSquaresBitboard & (1ULL << move)) == 0) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row up and 1 column left
    if (row < 7 && column > 0) {
        int move = position + 7;
        if ((board->getPieceType(move) == NONE || board->getPieceColor(move) != board->turn) && (board->controlledSquaresBitboard & (1ULL << move)) == 0) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row down and 1 column right
    if (row > 0 && column < 7) {
        int move = position - 7;
        ChessColor moveColor = board->getPieceColor(move);
        if ((moveColor == ChessColor::COLORNONE || moveColor == enemyColor) && (board->controlledSquaresBitboard & (1ULL << move)) == 0) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }
    // Check if king can move 1 row down and 1 column left
    if (row > 0 && column > 0) {
        int move = position - 9;
        ChessColor moveColor = board->getPieceColor(move);
        if ((moveColor == ChessColor::COLORNONE || moveColor == enemyColor) && (board->controlledSquaresBitboard & (1ULL << move)) == 0) {
            kingMoves.push_back(new Move(position, move, board));
        }
    }

    if (!board->inCheck) {
        // Check if king can castle
        if (board->turn == ChessColor::COLORWHITE) {
            // check if there is a rook on the king side
            if (board->getPieceType(63) == PieceType::ROOK && board->getPieceColor(63) == ChessColor::COLORWHITE) {
                // check if there are no pieces between the king and the rook
                if (board->getPieceType(61) == NONE && board->getPieceType(61) == NONE && (board->controlledSquaresBitboard & (1ULL << 61)) == 0 && (board->controlledSquaresBitboard & (1ULL << 62)) == 0) {
                    kingMoves.push_back(new Move(position, 62, board));
                }
            }
            // check if there is a rook on the queen side
            if (board->getPieceType(56) == PieceType::ROOK && board->getPieceColor(56) == ChessColor::COLORWHITE) {
                // check if there are no pieces between the king and the rook
                if (board->getPieceType(58) == NONE && board->getPieceType(59) == NONE && (board->controlledSquaresBitboard & (1ULL << 58)) == 0 && (board->controlledSquaresBitboard & (1ULL << 59)) == 0) {
                    kingMoves.push_back(new Move(position, 58, board));
                }
            }

        } else {
            // check if there is a rook on the king side
            if (board->getPieceType(7) == PieceType::ROOK && board->getPieceColor(7) == ChessColor::COLORBLACK) {
                // check if there are no pieces between the king and the rook
                if (board->getPieceType(5) == NONE && board->getPieceType(6) == NONE && (board->controlledSquaresBitboard & (1ULL << 5)) == 0 && (board->controlledSquaresBitboard & (1ULL << 6)) == 0) {
                    kingMoves.push_back(new Move(position, 6, board));
                }
            }
            // check if there is a rook on the queen side
            if (board->getPieceType(0) == PieceType::ROOK && board->getPieceColor(0) == ChessColor::COLORBLACK) {
                // check if there are no pieces between the king and the rook
                if (board->getPieceType(1) == NONE && board->getPieceType(2) == NONE && board->getPieceType(3) == NONE && (board->controlledSquaresBitboard & (1ULL << 1)) == 0 && (board->controlledSquaresBitboard & (1ULL << 2)) == 0 && (board->controlledSquaresBitboard & (1ULL << 3)) == 0) {
                    kingMoves.push_back(new Move(position, 2, board));
                }
            }
        }
    }
    
    board->moveMap[position] = kingMoves;
}

uint64_t MoveGenerator::GeneratePawnCaptures() const {
    uint64_t pawnCaptures = 0ULL;

    uint64_t pawnsBitboard = board->pawnsBitboard;
    // Loop through the Pawns bitboard
    while (pawnsBitboard) {
        // Get the position of the first pawn
        const int position = __builtin_ctzll(pawnsBitboard);
        if (board->getPieceColor(position) != board->turn) {
            pawnsBitboard &= pawnsBitboard - 1ULL;
            continue;
        }
        const int column = position % 8;
        if (board->turn == ChessColor::COLORWHITE) {
            if (column < 7) {
                pawnCaptures |= (1ULL << (position - 7));
            }
            if (column > 0) {
                pawnCaptures |= (1ULL << (position - 9));
            }
        } else {
            if (column > 0) {
                pawnCaptures |= (1ULL << (position + 9));
            }
            if (column < 7) {
                pawnCaptures |= (1ULL << (position + 7));
            }
        }
        pawnsBitboard &= pawnsBitboard - 1ULL;
    }
    return pawnCaptures;
}

uint64_t MoveGenerator::GenerateKnightCaptures() const {
    uint64_t knightCaptures = 0ULL;

    uint64_t knightsBitboard = board->knightsBitboard;

    while (knightsBitboard) {
        const int position = __builtin_ctzll(knightsBitboard);
        if (board->getPieceColor(position) != board->turn) {
            knightsBitboard &= knightsBitboard - 1ULL;
            continue;
        }
        // Generate all moves, don't check if they are valid
        const int row = position / 8;
        const int column = position % 8;

        // Check if knight can move 2 rows up and 1 column right
        if (row < 6 && column < 7) {
            int move = position + 17;
            knightCaptures |= (1ULL << move);
        }
        // Check if knight can move 2 rows up and 1 column left
        if (row < 6 && column > 0) {
            int move = position + 15;
            knightCaptures |= (1ULL << move);
        }
        // Check if knight can move 2 rows down and 1 column right
        if (row > 1 && column < 7) {
            int move = position - 15;
            knightCaptures |= (1ULL << move);
        }
        // Check if knight can move 2 rows down and 1 column left
        if (row > 1 && column > 0) {
            int move = position - 17;
            knightCaptures |= (1ULL << move);
        }
        // Check if knight can move 1 row up and 2 columns right
        if (row < 7 && column < 6) {
            int move = position + 10;
            knightCaptures |= (1ULL << move);
        }
        // Check if knight can move 1 row up and 2 columns left
        if (row < 7 && column > 1) {
            int move = position + 6;
            knightCaptures |= (1ULL << move);
        }
        // Check if knight can move 1 row down and 2 columns right
        if (row > 0 && column < 6) {
            int move = position - 6;
            knightCaptures |= (1ULL << move);
        }
        // Check if knight can move 1 row down and 2 columns left
        if (row > 0 && column > 1) {
            int move = position - 10;
            knightCaptures |= (1ULL << move);
        }
        knightsBitboard &= knightsBitboard - 1ULL;
    }
    return knightCaptures;
}

uint64_t MoveGenerator::GenerateBishopCaptures() const {
    uint64_t bishopCaptures = 0ULL;

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

            if (board->getPieceType(move) == NONE || board->getPieceType(move) == KING) {
                bishopCaptures |= (1ULL << move);
            } else {
                bishopCaptures |= (1ULL << move);
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
                if (!(board->movesThatBlockCheckBitboard & (1ULL << move))) {
                    continue;
                }
            }

            if (board->getPieceType(move) == NONE  || board->getPieceType(move) == KING) {
                bishopCaptures |= (1ULL << move);
            } else {
                bishopCaptures |= (1ULL << move);
                break;
            }
        }

        // Check if bishop can move down and right
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column + i > 7) {
                break;
            }
            int move = position - (7 * i);

            if (board->getPieceType(move) == NONE  || board->getPieceType(move) == KING) {
                bishopCaptures |= (1ULL << move);
            } else {
                bishopCaptures |= (1ULL << move);
                break;
            }
        }

        // Check if bishop can move down and left
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column - i < 0) {
                break;
            }
            int move = position - (9 * i);

            if (board->getPieceType(move) == NONE  || board->getPieceType(move) == KING) {
                bishopCaptures |= (1ULL << move);
            } else {
                bishopCaptures |= (1ULL << move);
                break;
            }
        }
        bishopsBitboard &= bishopsBitboard - 1ULL;
    }

    return bishopCaptures;
}

uint64_t MoveGenerator::GenerateRookCaptures() const {
    uint64_t rookCaptures = 0ULL;

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

            if (board->getPieceType(move) == NONE || board->getPieceType(move) == KING) {
                rookCaptures |= (1ULL << move);
            } else  {
                rookCaptures |= (1ULL << move);
                break;
            }
        }
        // Check if rook can move down
        for (int i = row - 1; i >= 0; i--) {
            int move = position - (8 * (row - i));

            if (board->getPieceType(move) == NONE || board->getPieceType(move) == KING) {
                rookCaptures |= (1ULL << move);
            } else {
                rookCaptures |= (1ULL << move);
                break;
            }
        }
        // Check if rook can move right
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);

            if (board->getPieceType(move) == NONE || board->getPieceType(move) == KING) {
                rookCaptures |= (1ULL << move);
            } else {
                rookCaptures |= (1ULL << move);
                break;
            }
        }
        // Check if rook can move left
        for (int i = column - 1; i >= 0; i--) {
            int move = position - (column - i);

            if (board->getPieceType(move) == NONE || board->getPieceType(move) == KING) {
                rookCaptures |= (1ULL << move);
            } else {
                rookCaptures |= (1ULL << move);
                break;
            }
        }
        rooksBitboard &= rooksBitboard - 1ULL;
    }

    return rookCaptures;
}

uint64_t MoveGenerator::GenerateQueenCaptures() const {
    uint64_t queenCaptures = 0ULL;

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
            queenCaptures |= (1ULL << move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE && board->getPieceType(move) != KING) {
                break;
            }
        }

        // Check if queen can move down
        for (int i = row - 1; i >= 0; i--) {
            int move = position - (8 * (row - i));
            queenCaptures |= (1ULL << move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE && board->getPieceType(move) != KING) {
                break;
            }
        }

        // Check if queen can move right
        for (int i = column + 1; i < 8; i++) {
            int move = position + (i - column);
            queenCaptures |= (1ULL << move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE && board->getPieceType(move) != KING) {
                break;
            }
        }

        // Check if queen can move left
        for (int i = column - 1; i >= 0; i--) {
            int move = position - (column - i);
            queenCaptures |= (1ULL << move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE && board->getPieceType(move) != KING) {
                break;
            }
        }

        // Check if queen can move up and right
        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column + i > 7) {
                break;
            }
            int move = position + (9 * i);
            queenCaptures |= (1ULL << move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE && board->getPieceType(move) != KING) {
                break;
            }
        }

        // Check if queen can move up and left
        for (int i = 1; i < 8; i++) {
            if (row + i > 7 || column - i < 0) {
                break;
            }
            int move = position + (7 * i);
            queenCaptures |= (1ULL << move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE && board->getPieceType(move) != KING) {
                break;
            }
        }

        // Check if queen can move down and right
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column + i > 7) {
                break;
            }
            int move = position - (7 * i);
            queenCaptures |= (1ULL << move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE && board->getPieceType(move) != KING) {
                break;
            }
        }

        // Check if queen can move down and left
        for (int i = 1; i < 8; i++) {
            if (row - i < 0 || column - i < 0) {
                break;
            }
            int move = position - (9 * i);
            queenCaptures |= (1ULL << move);
            // If there is a piece on the square, stop checking this direction
            if (board->getPieceType(move) != NONE && board->getPieceType(move) != KING) {
                break;
            }
        }
        queensBitboard &= queensBitboard - 1ULL;
    }
    return queenCaptures;
}

uint64_t MoveGenerator::GenerateKingCaptures() const {
    uint64_t kingCaptures = 0ULL;

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
            kingCaptures |= (1ULL << move);
        }
        // Check if king can move 1 row down
        if (row > 0) {
            int move = position - 8;
            kingCaptures |= (1ULL << move);
        }
        // Check if king can move 1 column right
        if (column < 7) {
            int move = position + 1;
            kingCaptures |= (1ULL << move);
        }
        // Check if king can move 1 column left
        if (column > 0) {
            int move = position - 1;
            kingCaptures |= (1ULL << move);
        }
        // Check if king can move 1 row up and 1 column right
        if (row < 7 && column < 7) {
            int move = position + 9;
            kingCaptures |= (1ULL << move);
        }
        // Check if king can move 1 row up and 1 column left
        if (row < 7 && column > 0) {
            int move = position + 7;
            kingCaptures |= (1ULL << move);
        }
        // Check if king can move 1 row down and 1 column right
        if (row > 0 && column < 7) {
            int move = position - 7;
            kingCaptures |= (1ULL << move);
        }
        // Check if king can move 1 row down and 1 column left
        if (row > 0 && column > 0) {
            int move = position - 9;
            kingCaptures |= (1ULL << move);
        }
        kingsBitboard &= kingsBitboard - 1ULL;
    }
    return kingCaptures;
}
