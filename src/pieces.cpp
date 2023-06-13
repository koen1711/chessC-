#include <raylib.h>
#include <iostream>
#include <string>
#include "pieces.h"
#include "board.h"
#include <vector>
#include <list>
#include <algorithm>

#define pawn std::string("pawn")
#define rook std::string("rook")
#define knight std::string("knight")
#define bishop std::string("bishop")
#define queen std::string("queen")
#define king std::string("king")
#define move_texture LoadTexture("assets/potential-move.png")
#define capture_texture LoadTexture("assets/selection.png")

void Piece::drawMoves(std::list<int> moves) {
    std::list<int> validMoves3;

    // loop through the moves and check if it is a check by using board.checkForCheckInPosition function
    for (auto it = moves.begin(); it != moves.end(); ) {
        int move = *it;
        // make the move
        std::map<int, Piece*> pieces = board.pieces;
        pieces[move] = this;
        pieces[position] = nullptr;
        int oldPosition = position;
        position = move;
        // check if the move puts us in check
        if (board.checkForCheckInPosition(pieces, color)) {
            it = moves.erase(it); // Erase the current element and get the iterator to the next element
        } else {
            validMoves3.push_back(move);
            ++it; // Move the iterator to the next element
        }
        // undo the move
        position = oldPosition;
    }

    moves = validMoves3;
    board.drawBoard();
    BeginDrawing();
    // clear board.potentialMoves
    board.potentialMoves.clear();
    for (int move : moves) {
        board.potentialMoves.push_back(move);
        board.selectedPiece = this;
        int x = board.boardPositions[move][0];
        int y = board.boardPositions[move][1];
        DrawCircle(x + (82.5 /2), y + (82.5 /2), 30 / 2, GRAY);
    }
    EndDrawing();
}



std::list<int> Pawn::getMoves(std::map<int, Piece*> pieces) {
    std::list<int> moves = getPawnMoves(pieces);

    std::list<int> validMoves;
    for (int move : moves) {
        if (pieces[move] == nullptr || pieces[move]->color != color) {
            validMoves.push_back(move);
        }
    }
    moves = validMoves;

    bool check = false;
    // get our king
    if (color == std::string("white")) {
        check = board.whiteKing->check;
    } else {
        check = board.blackKing->check;
    }

    if (check == true) {
        // print all the moves that block the check
        std::list<int> validMoves2;
        // print all the moves that block the check
        for (auto it = moves.begin(); it != moves.end(); ) {
            int move = *it;
            if (std::find(board.movesThatBlockCheck.begin(), board.movesThatBlockCheck.end(), move) != board.movesThatBlockCheck.end()) {
                validMoves2.push_back(move);
                ++it; // Move the iterator to the next element
            } else {
                it = moves.erase(it); // Erase the current element and get the iterator to the next element
            }
        }
        moves = validMoves2;
    }



    return moves;
}

std::list<int> Rook::getMoves(std::map<int, Piece*> pieces) {
    std::list<int> moves = getRookMoves(pieces);

    std::list<int> validMoves;
    for (int move : moves) {
        if (pieces[move] == nullptr || pieces[move]->color != this->color) {
            validMoves.push_back(move);
        }
    }
    moves = validMoves;

    bool check = false;
    // get our king
    if (color == std::string("white")) {
        check = board.whiteKing->check;
    } else {
        check = board.blackKing->check;
    }

    if (check == true) {
        // print all the moves that block the check
        std::list<int> validMoves2;
        // print all the moves that block the check
        for (auto it = moves.begin(); it != moves.end(); ) {
            int move = *it;
            if (std::find(board.movesThatBlockCheck.begin(), board.movesThatBlockCheck.end(), move) != board.movesThatBlockCheck.end()) {
                validMoves2.push_back(move);
                ++it; // Move the iterator to the next element
            } else {
                it = moves.erase(it); // Erase the current element and get the iterator to the next element
            }
        }
        moves = validMoves2;
    }


    return moves;
}

std::list<int> Bishop::getMoves(std::map<int, Piece*> pieces) {
    std::list<int> moves;

    std::list<int> bishopMoves = getBishopMoves(pieces);

    if (pinned == true) {
        return moves;
    }

    for (int move : bishopMoves) {
        // check if move is possible
        if (pieces[move] == nullptr) {
            moves.push_back(move);
        }
        else if (pieces[move]->color != this->color) {
            moves.push_back(move);
        }
    }

    bool check = false;
    // get our king
    if (color == std::string("white")) {
        check = board.whiteKing->check;
    } else {
        check = board.blackKing->check;
    }

    if (check == true) {
        // print all the moves that block the check
        std::list<int> validMoves2;
        // print all the moves that block the check
        for (auto it = moves.begin(); it != moves.end(); ) {
            int move = *it;
            if (std::find(board.movesThatBlockCheck.begin(), board.movesThatBlockCheck.end(), move) != board.movesThatBlockCheck.end()) {
                validMoves2.push_back(move);
                ++it; // Move the iterator to the next element
            } else {
                it = moves.erase(it); // Erase the current element and get the iterator to the next element
            }
        }
        moves = validMoves2;
    }


    return moves;
}

std::list<int> Knight::getMoves(std::map<int, Piece*> pieces) {
    std::list<int> moves = getKnightMoves(pieces);

    std::list<int> validMoves;
    for (int move : moves) {
        if (pieces[move] == nullptr || pieces[move]->color != this->color) {
            if (move < 64 && move >= 0) {
                validMoves.push_back(move);
            }
        }
    }
    moves = validMoves;

    bool check = false;
    // get our king
    if (color == std::string("white")) {
        check = board.whiteKing->check;
    } else {
        check = board.blackKing->check;
    }



    if (check == true) {
        // print all the moves that block the check
        std::list<int> validMoves2;
        // print all the moves that block the check
        for (auto it = moves.begin(); it != moves.end(); ) {
            int move = *it;
            if (std::find(board.movesThatBlockCheck.begin(), board.movesThatBlockCheck.end(), move) != board.movesThatBlockCheck.end()) {
                validMoves2.push_back(move);
                ++it; // Move the iterator to the next element
            } else {
                it = moves.erase(it); // Erase the current element and get the iterator to the next element
            }
        }
        moves = validMoves2;
    }


    return moves;
}

std::list<int> Queen::getMoves(std::map<int, Piece*> pieces) {
    std::list<int> moves;
    std::list<int> rookMoves = getRookMoves(pieces);
    std::list<int> bishopMoves = getBishopMoves(pieces);
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());

    // remove the moves that take it;s own color

    std::list<int> validMoves;
    for (int move : moves) {
        if (board.pieces[move] == nullptr || board.pieces[move]->color != this->color) {
            validMoves.push_back(move);
        }
    }
    moves = validMoves;

    // check if the king is in check
    validMoves.clear();

    bool check = false;
    // get our king
    if (color == std::string("white")) {
        check = board.whiteKing->check;
    } else {
        check = board.blackKing->check;
    }

    if (check == true) {
        // print all the moves that block the check
        std::list<int> validMoves2;
        // print all the moves that block the check
        for (auto it = moves.begin(); it != moves.end(); ) {
            int move = *it;
            if (std::find(board.movesThatBlockCheck.begin(), board.movesThatBlockCheck.end(), move) != board.movesThatBlockCheck.end()) {
                validMoves2.push_back(move);
                ++it; // Move the iterator to the next element
            } else {
                it = moves.erase(it); // Erase the current element and get the iterator to the next element
            }
        }
        moves = validMoves2;
    }

    return moves;
}

std::list<int> King::getMoves(std::map<int, Piece*> pieces) {
    std::list<int> kingMoves = getKingMoves(pieces);
    std::list<int> validMoves;

    for (int move : kingMoves) {
        if (pieces[move] == nullptr || pieces[move]->color != color) {
            validMoves.push_back(move);
        }
    }
    kingMoves = validMoves;

    std::list<int> validMoves2;
    for (int move : kingMoves) {
        if (std::find(board.movesThatBlockCheck.begin(), board.movesThatBlockCheck.end(), move) == board.movesThatBlockCheck.end()) {
            validMoves2.push_back(move);
        }
    }

    kingMoves = validMoves2;

    return kingMoves;
}





// MAKE MOVE
void Pawn::makeMove(int pos) {

    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
    }

    // check if there is a piece in the position if so free the memory where it lives
    if (board.pieces[pos] != nullptr) {
        delete board.pieces[pos];
    }

    // check if there is a piece left or right of the pawn for en passant
    if (pos == position - 16 || pos == position + 16) {
        if (board.pieces[pos -1 ] != nullptr && board.pieces[pos -1 ]->color != color && board.pieces[pos -1 ]->enPassantRight == false) {
            board.pieces[pos -1 ]->enPassantRight = true;
        } else if (board.pieces[pos +1 ] != nullptr && board.pieces[pos +1 ]->color != color && board.pieces[pos +1 ]->enPassantLeft == false) {
            board.pieces[pos +1 ]->enPassantLeft = true;
        }
    }
    int oldPos = position;
    if ((pos == oldPos - 7 || pos == oldPos - 9 || pos == oldPos + 7 || pos == oldPos + 9) && (enPassantLeft == true || enPassantRight == true)) {
        if (color == std::string("white")) {
            delete board.pieces[pos + 8];
            board.pieces[pos + 8] = nullptr;
        } else {
            delete board.pieces[pos - 8];
            board.pieces[pos - 8] = nullptr;
        }
    }

    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;

    enPassantRight = false;
    enPassantLeft = false;

    // check if the pawn is en passant


    // check if the pawn is at the end of the board
    if (color == std::string("white") && board.getRow(pos) == 0) {
        board.promotePawn(this);
    } else if (color == std::string("black") && board.getRow(pos) == 7) {
        board.promotePawn(this);
    }
}

void Bishop::makeMove(int pos) {

    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
    }

    // check if there is a piece in the position if so free the memory where it lives
    if (board.pieces[pos] != nullptr) {
        delete board.pieces[pos];
    }

    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;
}

void Rook::makeMove(int pos) {
    if (std::string("white") == color) {
        //set the whiteRightRookMoved or whiteLeftRookMoved to true
        if (board.whiteRightRookMoved == false && position == 7) {
            board.whiteRightRookMoved = true;
        } else if (board.whiteLeftRookMoved == false && position == 0) {
            board.whiteLeftRookMoved = true;
        }
    } else {
        //set the blackRightRookMoved or blackLeftRookMoved to true
        if (board.blackRightRookMoved == false && position == 63) {
            board.blackRightRookMoved = true;
        } else if (board.blackLeftRookMoved == false && position == 56) {
            board.blackLeftRookMoved = true;
        }
    }


    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
    }

    // check if there is a piece in the position if so free the memory where it lives
    if (board.pieces[pos] != nullptr) {
        delete board.pieces[pos];
    }

    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;
}

void Knight::makeMove(int pos) {

    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
    }

    // check if there is a piece in the position if so free the memory where it lives
    if (board.pieces[pos] != nullptr) {
        delete board.pieces[pos];
    }

    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;
}

void Queen::makeMove(int pos) {

    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
    }

    // check if there is a piece in the position if so free the memory where it lives
    if (board.pieces[pos] != nullptr) {
        delete board.pieces[pos];
    }

    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;
}

void King::makeMove(int pos) {
    if (color == std::string("white")) {
        board.whiteKingDoNotMove.push_back(pos);
    } else {
        board.blackKingDoNotMove.push_back(pos);
    }

    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
    }

    // check if there is a piece in the position if so free the memory where it lives
    if (board.pieces[pos] != nullptr) {
        delete board.pieces[pos];
    }

    if (std::abs(pos - position) == 2) {
        if (pos > position) {
            // king side castle
            if (color == std::string("white")) {
                board.whiteRightRookMoved = true;
                board.pieces[63]->makeMove(61);
            } else {
                board.blackRightRookMoved = true;
                board.pieces[7]->makeMove(5);
            }
        } else {
            // queen side castle
            if (color == std::string("white")) {
                board.whiteLeftRookMoved = true;
                board.pieces[56]->makeMove(59);
            } else {
                board.blackLeftRookMoved = true;
                board.pieces[0]->makeMove(3);
            }
        }
    }

    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;

    // check if the king is castling
    if (color == std::string("white")) {
        board.whiteKingMoved = true;
    } else {
        board.blackKingMoved = true;
    }
}



// GET MOVES
std::list<int> Piece::getBishopMoves(std::map<int, Piece*> pieces) {
    std::list<int> moves;
    bool down_left_allowed = true;
    bool down_right_allowed = true;
    bool up_left_allowed = true;
    bool up_right_allowed = true;

    if (board.leftEdge.find(position) != board.leftEdge.end() || board.bottomEdge.find(position) != board.bottomEdge.end()) {
        down_left_allowed = false;
    }

    if (board.rightEdge.find(position) != board.rightEdge.end() || board.bottomEdge.find(position) != board.bottomEdge.end()) {
        down_right_allowed = false;
    }

    if (board.leftEdge.find(position) != board.leftEdge.end() || board.topEdge.find(position) != board.topEdge.end()) {
        up_left_allowed = false;
    }

    if (board.rightEdge.find(position) != board.rightEdge.end() || board.topEdge.find(position) != board.topEdge.end()) {
        up_right_allowed = false;
    }

    if (down_left_allowed) {
        for (int i = position + 7; i < 64; i += 7) {
            if (pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                moves.push_back(i);
                break;
            }
            if (board.leftEdge.find(i) != board.leftEdge.end() || board.bottomEdge.find(i) != board.bottomEdge.end()) {
                break;
            }
        }
    }

    if (down_right_allowed) {
        for (int i = position + 9; i < 64; i += 9) {
            if (pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                moves.push_back(i);
                break;
            }
            if (board.rightEdge.find(i) != board.rightEdge.end() || board.bottomEdge.find(i) != board.bottomEdge.end()) {
                break;
            }
        }
    }

    if (up_left_allowed) {
        for (int i = position - 9; i >= 0; i -= 9) {
            if (pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                moves.push_back(i);
                break;
            }
            if (board.leftEdge.find(i) != board.leftEdge.end() || board.topEdge.find(i) != board.topEdge.end()) {
                break;
            }
        }
    }

    if (up_right_allowed) {
        for (int i = position - 7; i >= 0; i -= 7) {
            if (pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                moves.push_back(i);
                break;
            }
            if (board.rightEdge.find(i) != board.rightEdge.end() || board.topEdge.find(i) != board.topEdge.end()) {
                break;
            }
        }
    }

    return moves;
}

std::list<int> Piece::getRookMoves(std::map<int, Piece*> pieces) {
    std::list<int> moves;
    bool left_allowed = true;
    bool right_allowed = true;
    bool up_allowed = true;
    bool down_allowed = true;

    if (board.leftEdge.find(position) != board.leftEdge.end()) {
        left_allowed = false;
    }
    if (board.rightEdge.find(position) != board.rightEdge.end()) {
        right_allowed = false;
    }
    if (board.topEdge.find(position) != board.topEdge.end()) {
        up_allowed = false;
    }
    if (board.bottomEdge.find(position) != board.bottomEdge.end()) {
        down_allowed = false;
    }

    if (left_allowed) {
        for (int i = position - 1; i >= 0; i--) {
            if (pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                moves.push_back(i);
                break;
            }
            // check if we are at the left edge
            if (board.leftEdge.find(i) != board.leftEdge.end()) {
                break;
            }
        }
    }
    if (right_allowed) {
        for (int i = position + 1; i < 64; i++) {
            if (pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                moves.push_back(i);
                break;
            }
            // check if we are at the right edge
            if (board.rightEdge.find(i) != board.rightEdge.end()) {
                break;
            }
        }
    }
    if (up_allowed) {
        for (int i = position - 8; i >= 0; i -= 8) {
            if (pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                moves.push_back(i);
                break;
            }

        }
    }
    if (down_allowed) {
        for (int i = position + 8; i < 64; i += 8) {
            if (pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                // print what is at this position
                moves.push_back(i);
                break;
            }
        }
    }

    return moves;
}

std::list<int> Piece::getKingMoves(std::map<int, Piece*> pieces) {

    bool up_allowed = true;
    bool down_allowed = true;
    bool left_allowed = true;
    bool right_allowed = true;

    int row = board.getRow(position);
    int col = board.getColumn(position);

    if (board.topEdge.find(position) != board.topEdge.end()) {
        up_allowed = false;
    }

    if (board.bottomEdge.find(position) != board.bottomEdge.end()) {
        down_allowed = false;
    }

    if (board.leftEdge.find(position) != board.leftEdge.end()) {
        left_allowed = false;
    }

    if (board.rightEdge.find(position) != board.rightEdge.end()) {
        right_allowed = false;
    }

    std::list<int> movesKing;

    if (down_allowed) {
        movesKing.push_back(board.getPosition(row + 1, col));
        if (left_allowed) {
            movesKing.push_back(board.getPosition(row + 1, col - 1));
        }
        if (right_allowed) {
            movesKing.push_back(board.getPosition(row + 1, col + 1));
        }
    }

    if (right_allowed) {
        movesKing.push_back(board.getPosition(row, col + 1));
    }
    if (left_allowed) {
        movesKing.push_back(board.getPosition(row, col - 1));
    }
    if (up_allowed) {
        movesKing.push_back(board.getPosition(row - 1, col));
        if (left_allowed) {
            movesKing.push_back(board.getPosition(row - 1, col - 1));
        }
        if (right_allowed) {
            movesKing.push_back(board.getPosition(row - 1, col + 1));
        }
    }

    // check if castling is possible
    if (color == "white") {
        if (board.whiteKingMoved == false && check == false && position == 60) {
            if (board.whiteLeftRookMoved == false) {
                if (pieces[57] == nullptr && pieces[58] == nullptr && pieces[59] == nullptr) {
                    movesKing.push_back(58);
                }
            }
            if (board.whiteRightRookMoved == false) {
                if (pieces[61] == nullptr && pieces[62] == nullptr) {
                    movesKing.push_back(62);
                }
            }
        }
    } else {
        if (board.blackKingMoved == false && check == false && position == 4) {
            if (board.blackLeftRookMoved == false) {
                if (pieces[1] == nullptr && pieces[2] == nullptr && pieces[3] == nullptr) {
                    movesKing.push_back(2);
                }
            }
            if (board.blackRightRookMoved == false) {
                if (pieces[5] == nullptr && pieces[6] == nullptr) {
                    movesKing.push_back(6);
                }
            }
        }
    }

    return movesKing;
}

std::list<int> Piece::getPawnMoves(std::map<int, Piece*> pieces) {
    std::list<int> moves;

    bool left_up_allowed = true;
    bool right_up_allowed = true;

    if (board.leftEdge.find(position) != board.leftEdge.end()) {
        left_up_allowed = false;
    }

    if (board.rightEdge.find(position) != board.rightEdge.end()) {
        right_up_allowed = false;
    }

    if (color == "white") {
        if (pieces[position - 8] == nullptr) {
            moves.push_back(position - 8);
            if (position < 56 && position > 47 && pieces[position - 16] == nullptr) {
                moves.push_back(position - 16);
            }
        }
        if (left_up_allowed && ((pieces[position - 9] != nullptr && pieces[position - 9]->color != color) || (enPassantLeft))) {
            moves.push_back(position - 9);
        }

        if (right_up_allowed && ((pieces[position - 7] != nullptr && pieces[position - 7]->color != color) || (enPassantRight))) {
            moves.push_back(position - 7);
        }
    } else {
        if (pieces[position + 8] == nullptr) {
            moves.push_back(position + 8);
            if (position < 16 && position > 7 && pieces[position + 16] == nullptr) {
                moves.push_back(position + 16);
            }
        }

        if (right_up_allowed && ((pieces[position + 9] != nullptr && pieces[position + 9]->color != color) || (enPassantRight))) {
            moves.push_back(position + 9);
        }

        if (left_up_allowed && ((pieces[position + 7] != nullptr && pieces[position + 7]->color != color) || (enPassantLeft))) {
            moves.push_back(position + 7);
        }
    }

    return moves;
}

std::list<int> Piece::getKnightMoves(std::map<int, Piece*> pieces) {
    std::list<int> moves;

    int row = board.getRow(position);
    int column = board.getColumn(position);

    // Check each possible knight move and add valid positions to the moves list
    if (row >= 2) {
        if (column >= 1) {
            int pos = board.getPosition(row - 2, column - 1);
            moves.push_back(pos);
        }
        if (column <= 6) {
            int pos = board.getPosition(row - 2, column + 1);
            moves.push_back(pos);
        }
    }
    if (row >= 1) {
        if (column >= 2) {
            int pos = board.getPosition(row - 1, column - 2);
            moves.push_back(pos);
        }
        if (column <= 5) {
            int pos = board.getPosition(row - 1, column + 2);
            moves.push_back(pos);
        }
    }
    if (row <= 6) {
        if (column >= 1) {
            int pos = board.getPosition(row + 2, column - 1);
            moves.push_back(pos);
        }
        if (column <= 6) {
            int pos = board.getPosition(row + 2, column + 1);
            moves.push_back(pos);
        }
    }
    if (row <= 5) {
        if (column >= 2) {
            int pos = board.getPosition(row + 1, column - 2);
            moves.push_back(pos);
        }
        if (column <= 5) {
            int pos = board.getPosition(row + 1, column + 2);
            moves.push_back(pos);
        }
    }

    return moves;
}



// POTENTIAL MOVES
    std::list<int> Piece::getPotentialMoves(std::map<int, Piece*> pieces) {return std::list<int>();}
    std::list<int> Pawn::getPotentialMoves(std::map<int, Piece*> pieces) {return getPawnMoves(pieces);}
    std::list<int> Rook::getPotentialMoves(std::map<int, Piece*> pieces) {return getRookMoves(pieces);}
    std::list<int> Bishop::getPotentialMoves(std::map<int, Piece*> pieces) {return getBishopMoves(pieces);}
    std::list<int> Knight::getPotentialMoves(std::map<int, Piece*> pieces) {return getKnightMoves(pieces);}
    std::list<int> Queen::getPotentialMoves(std::map<int, Piece*> pieces) {
        std::list<int> moves;
        std::list<int> rookMoves = getRookMoves(pieces);
        std::list<int> bishopMoves = getBishopMoves(pieces);
        moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
        moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
        return moves;
    }
    std::list<int> King::getPotentialMoves(std::map<int, Piece*> pieces) {return getKingMoves(pieces);}



// USELESS OVERRIDES
    void Piece::makeMove(int pos) {}
    std::list<int> Piece::getMoves(std::map<int, Piece*> pieces) {return std::list<int>();}



// ONE LINERS
    void Piece::move(std::map<int, Piece*> pieces) {drawMoves(this->getMoves(pieces));}
    King::King(int position, std::string color, char fenName, Board& board): Piece(position, color, fenName, board) {name = king;}
    Knight::Knight(int position, std::string color, char fenName, Board& board): Piece(position, color, fenName, board) {name = knight;}
    Bishop::Bishop(int position, std::string color, char fenName, Board& board): Piece(position, color, fenName, board) {name = bishop;}
    Rook::Rook(int position, std::string color, char fenName, Board& board): Piece(position, color, fenName, board) {name = rook;}
    Queen::Queen(int position, std::string color, char fenName, Board& board): Piece(position, color, fenName, board) {name = queen;}
    Pawn::Pawn(int position, std::string color, char fenName, Board& board): Piece(position, color, fenName, board) {name = pawn;}
    Piece::Piece(int position, std::string color, char fenName, Board& board): position(position), color(color), fenName(fenName), board(board) {}
    Piece::~Piece() {}
    Pawn::~Pawn() {}
    Rook::~Rook() {}
    Knight::~Knight() {}
    Bishop::~Bishop() {}
    Queen::~Queen() {}
    King::~King() {}