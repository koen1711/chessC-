#include <iostream>
#include <raylib.h>
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

Piece::Piece(int position, std::string color, char fenName, Board& board)
    : position(position), color(color), fenName(fenName), board(board) {}

Piece::~Piece() {}

Pawn::Pawn(int position, std::string color, char fenName, Board& board)
    : Piece(position, color, fenName, board) {
    name = pawn;
}

Pawn::~Pawn() {}

Rook::Rook(int position, std::string color, char fenName, Board& board)
    : Piece(position, color, fenName, board) {
    name = rook;
}

Rook::~Rook() {}


Knight::Knight(int position, std::string color, char fenName, Board& board)
    : Piece(position, color, fenName, board) {
    name = knight;
}

Knight::~Knight() {}


Bishop::Bishop(int position, std::string color, char fenName, Board& board)
    : Piece(position, color, fenName, board) {
    name = bishop;
}

Bishop::~Bishop() {}

Queen::Queen(int position, std::string color, char fenName, Board& board)
    : Piece(position, color, fenName, board) {
    name = queen;
}

Queen::~Queen() {}

King::King(int position, std::string color, char fenName, Board& board)
    : Piece(position, color, fenName, board) {
    name = king;
}

King::~King() {}

void Piece::drawMoves(std::list<int> moves) {
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

void Piece::move() {
    drawMoves(this->getMoves());
}

std::list<int> Pawn::getMoves() {
    return getPawnMoves();
}

std::list<int> Rook::getMoves() {
    return getRookMoves();
}

std::list<int> Bishop::getMoves() {
    std::list<int> moves;

    std::list<int> bishopMoves = getBishopMoves();

    if (pinned == true) {
        return moves;
    }

    for (int move : bishopMoves) {
        // check if move is possible
        if (board.pieces[move] == nullptr) {
            moves.push_back(move);
        }
        else if (board.pieces[move]->color != this->color) {
            moves.push_back(move);
        }
    }


    return moves;
}

std::list<int> Knight::getMoves() {
    return getKnightMoves();
}

std::list<int> Queen::getMoves() {
    std::list<int> moves;
    std::list<int> rookMoves = getRookMoves();
    std::list<int> bishopMoves = getBishopMoves();
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
        if (board.whiteKing->check == true) {
            check = true;
        }
    } else {
        if (board.blackKing->check == true) {
            check = true;
        }
    }

    if (check == true) {
        std::list<int> validMoves2;
        for (auto it = moves.begin(); it != moves.end(); ) {
            int move = *it;
            if (std::find(board.movesThatBlockCheck.begin(), board.movesThatBlockCheck.end(), move) == board.movesThatBlockCheck.end()) {
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

std::list<int> King::getMoves() {
    return getKingMoves();
}

std::list<int> Piece::getMoves()
{
    return std::list<int>();
}

std::list<int> Piece::getPotentialMoves() {
    return std::list<int>();
}

std::list<int> Pawn::getPotentialMoves() {
    return getPawnMoves();
}

std::list<int> Rook::getPotentialMoves() {
    return getRookMoves();
}

std::list<int> Bishop::getPotentialMoves() {
    return getBishopMoves();
}

std::list<int> Knight::getPotentialMoves() {
    return getKnightMoves();
}

std::list<int> Queen::getPotentialMoves() {
    std::list<int> moves;
    std::list<int> rookMoves = getRookMoves();
    std::list<int> bishopMoves = getBishopMoves();
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    return moves;
}

std::list<int> King::getPotentialMoves() {
    return getKingMoves();
}

void Pawn::makeMove(int pos) {

    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
    }

    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;
}

void Bishop::makeMove(int pos) {

    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
    }

    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;
}

void Rook::makeMove(int pos) {

    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
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

    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;
}

void King::makeMove(int pos) {

    if (this == nullptr) {
        return;
    }

    if (pos < 0 || pos >= board.pieces.size()) {
        return;
    }
    board.pieces[pos] = this;
    board.pieces.erase(position);
    position = pos;
}

void Piece::makeMove(int pos) {
}

std::list<int> Piece::getBishopMoves() {
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
            if (board.pieces[i] == nullptr) {
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
            if (board.pieces[i] == nullptr) {
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
            if (board.pieces[i] == nullptr) {
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
            if (board.pieces[i] == nullptr) {
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

std::list<int> Piece::getRookMoves() {
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
            if (board.pieces[i] == nullptr) {
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
            if (board.pieces[i] == nullptr) {
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
            if (board.pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                moves.push_back(i);
                break;
            }

        }
    }
    if (down_allowed) {
        for (int i = position + 8; i < 64; i += 8) {
            if (board.pieces[i] == nullptr) {
                moves.push_back(i);
            } else {
                moves.push_back(i);
                break;
            }
        }
    }

    return moves;
}

std::list<int> Piece::getKingMoves() {
    std::list<int> moves;

    if (board.pieces[position + 1] == nullptr) {
        moves.push_back(position + 1);
    }
    if (board.pieces[position - 1] == nullptr) {
        moves.push_back(position - 1);
    }
    if (board.pieces[position + 8] == nullptr) {
        moves.push_back(position + 8);
    }
    if (board.pieces[position - 8] == nullptr) {
        moves.push_back(position - 8);
    }
    if (board.pieces[position + 7] == nullptr) {
        moves.push_back(position + 7);
    }
    if (board.pieces[position - 7] == nullptr) {
        moves.push_back(position - 7);
    }
    if (board.pieces[position + 9] == nullptr) {
        moves.push_back(position + 9);
    }
    if (board.pieces[position - 9] == nullptr) {
        moves.push_back(position - 9);
    }

    return moves;
}

std::list<int> Piece::getPawnMoves() {

    std::list<int> moves;

    if (color == "white") {
        if (board.pieces[position - 8] == nullptr) {
            moves = {position - 8};
            if (position < 56 && position > 47 && board.pieces[position - 16] == nullptr) {
                moves = {position - 8, position - 16};
            }
            if (board.pieces[position - 7] != nullptr && board.pieces[position - 7]->color != color) {
                moves.push_back(position - 7);
            }
            if (board.pieces[position - 9] != nullptr && board.pieces[position - 9]->color != color) {
                moves.push_back(position - 9);
            }
        }
    } else {

        if (board.pieces[position + 8] == nullptr) {
            moves = {position + 8};
            if (position < 16 && position > 7 && board.pieces[position + 16] == nullptr) {
                moves = {position + 8, position + 16};
            }
            if (board.pieces[position + 7] != nullptr && board.pieces[position + 7]->color != color) {
                moves.push_back(position + 7);
            }
            if (board.pieces[position + 9] != nullptr && board.pieces[position + 9]->color != color) {
                moves.push_back(position + 9);
            }
        }
    }

    return moves;
}

std::list<int> Piece::getKnightMoves() {
    std::list<int> moves;

    if (board.pieces[position + 17] == nullptr) {
        moves.push_back(position + 17);
    }
    if (board.pieces[position + 15] == nullptr) {
        moves.push_back(position + 15);
    }
    if (board.pieces[position - 17] == nullptr) {
        moves.push_back(position - 17);
    }
    if (board.pieces[position - 15] == nullptr) {
        moves.push_back(position - 15);
    }
    if (board.pieces[position + 10] == nullptr) {
        moves.push_back(position + 10);
    }
    if (board.pieces[position + 6] == nullptr) {
        moves.push_back(position + 6);
    }
    if (board.pieces[position - 10] == nullptr) {
        moves.push_back(position - 10);
    }
    if (board.pieces[position - 6] == nullptr) {
        moves.push_back(position - 6);
    }

    return moves;
}

