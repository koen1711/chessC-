#ifndef CHESSC___MOVE_H
#define CHESSC___MOVE_H

#include "../Pieces/Pieces.h"
#include "../Board/Board.h"
#include "../Types/Types.h"

class Board;

class Piece;

class Move {
public:
    Move(int from, int to, Board* board);
    Piece* movedPiece = nullptr;
    Piece* capturedPiece = nullptr;
    bool firstMove = false;
    bool isCastle = false;
    bool isEnPassant = false;
    bool isPromotion = false;
    bool isPawnDoubleMove = false;
    PieceType promotionPiece = PieceType::QUEEN;
    int fromSquare;
    int toSquare;
};

#endif //CHESSC___MOVE_H
