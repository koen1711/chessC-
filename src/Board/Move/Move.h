#ifndef CHESSC___MOVE_H
#define CHESSC___MOVE_H

#include "../Pieces/Pieces.h"
#include "../Board.h"
#include "../Types/Types.h"

class Board;

class Piece;

class Move {
public:
    Move(int from, int to, Board* board);
    Piece* movedPiece;
    Piece* capturedPiece;
    bool isCastle;
    bool isEnPassant;
    bool isPromotion;
    PieceType promotionPiece;
    int fromSquare;
    int toSquare;
};

#endif //CHESSC___MOVE_H
