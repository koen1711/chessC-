#ifndef CHESSC___PIECES_H
#define CHESSC___PIECES_H

#include <string>
#include <vector>
#include <map>
#include "../Move/Move.h"
#include "../Types/Types.h"

class Move;

class Piece {
    public:
        PieceType name;
        ChessColor color;
        int position;
        std::map<int, Move*> moves = {};
        bool enPassantLeft = false;
        bool enPassantRight = false;

        Piece(PieceType name, ChessColor color, int square);
        std::map<int, Move*> getMoves();

        bool pinned = false;
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
