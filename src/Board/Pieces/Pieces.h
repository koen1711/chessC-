#ifndef CHESSC___PIECES_H
#define CHESSC___PIECES_H

#include "../Move/Move.h"
#include "../Types/Types.h"
#include <vector>

class Move;

class Piece {
public:
    PieceType name;
    ChessColor color;
    int position;
    std::vector<Move*> moves = {};
    PieceFlags* pFlags = new PieceFlags();
    Directions pinDirection = Directions::VERTICALUP;

    Piece(PieceType name, ChessColor color, int square);
    std::vector<Move*> getMoves();
};


class Pawn : public Piece {
    public:
        Pawn(ChessColor color, int square);
};

class Knight : public Piece {
    public:
        Knight(ChessColor color, int square);
};

class Bishop : public Piece {
    public:
        Bishop(ChessColor color, int square);
};

class Rook : public Piece {
    public:
        Rook(ChessColor color, int square);
};

class Queen : public Piece {
    public:
        Queen(ChessColor color, int square);
};

class King : public Piece {
    public:
        King(ChessColor color, int square);
};

#endif //CHESSC___PIECES_H
