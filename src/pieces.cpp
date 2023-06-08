#include <iostream>
#include <raylib.h>
#include <string>
#include "pieces.h"
#include "board.h"

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
    BeginDrawing();
    for (int move : moves) {
        int x = board.boardPositions[move][0];
        int y = board.boardPositions[move][1];
        //DrawTexture(move_texture, x * 82.5, y * 82.5, RED);
        DrawCircle(x + (82.5 /2), y + (82.5 /2), 30 / 2, GRAY);
    }
    std::cout << "Drawing moves" << std::endl;
    EndDrawing();
}

void Piece::move() {
    drawMoves(this->getMoves());
}

std::list<int> Pawn::getMoves() {
    std::list<int> moves;
    if (color == "white") {
        std::cout << "White pawn" << std::endl;
        if (board.pieces[position + 8] == nullptr) {
            std::cout << "No piece in front" << std::endl;
            moves = {position + 8};
            if (position < 56 && position > 47 && board.pieces[position + 16] == nullptr) {
                moves = {position - 8, position - 16};
            }
        } else {
            std::cout << "Piece in front" << typeid(this).name() << std::endl;
        }
    } else {
        moves = {20, 21};
    }
    return moves;
}

std::list<int> Rook::getMoves() {
    std::list<int> moves;
    int x = board.boardPositions[position][0];
    int y = board.boardPositions[position][1];
    for (int i = 0; i < 8; i++) {
        if (i != x) {
            moves.push_back(board.boardPositions[i][y]);
        }
        if (i != y) {
            moves.push_back(board.boardPositions[x][i]);
        }
    }
    return moves;
}

std::list<int> Bishop::getMoves() {
    std::list<int> moves;
    int x = board.boardPositions[position][0];
    int y = board.boardPositions[position][1];
    for (int i = 0; i < 8; i++) {
        if (i != x) {
            moves.push_back(board.boardPositions[i][y]);
        }
        if (i != y) {
            moves.push_back(board.boardPositions[x][i]);
        }
    }
    return moves;
}

std::list<int> Knight::getMoves() {
    std::list<int> moves;
    int x = board.boardPositions[position][0];
    int y = board.boardPositions[position][1];
    for (int i = 0; i < 8; i++) {
        if (i != x) {
            moves.push_back(board.boardPositions[i][y]);
        }
        if (i != y) {
            moves.push_back(board.boardPositions[x][i]);
        }
    }
    return moves;
}

std::list<int> Queen::getMoves() {
    std::list<int> moves;
    int x = board.boardPositions[position][0];
    int y = board.boardPositions[position][1];
    for (int i = 0; i < 8; i++) {
        if (i != x) {
            moves.push_back(board.boardPositions[i][y]);
        }
        if (i != y) {
            moves.push_back(board.boardPositions[x][i]);
        }
    }
    return moves;
}

std::list<int> King::getMoves() {
    std::list<int> moves;
    int x = board.boardPositions[position][0];
    int y = board.boardPositions[position][1];
    for (int i = 0; i < 8; i++) {
        if (i != x) {
            moves.push_back(board.boardPositions[i][y]);
        }
        if (i != y) {
            moves.push_back(board.boardPositions[x][i]);
        }
    }
    return moves;
}

std::list<int> Piece::getMoves()
{
    std::cout << "Piece get moves" << std::endl;
    // Implementation logic for the base class Piece
    // Return the list of possible moves for the piece
    return std::list<int>();
}
