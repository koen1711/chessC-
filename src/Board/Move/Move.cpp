#include "Move.h"
#include "../Setup/Setup.h"

Move::Move(int from, int to, Board* board) : fromSquare(from), toSquare(to)  {
    movedPiece = board->board[from];
    capturedPiece = board->board[to];
    isCastle = false;
    isEnPassant = false;
    isPromotion = false;

    // Check if the move is a castle
    if (movedPiece->name == PieceType::KING && abs(from - to) == 2) {
        isCastle = true;
    }

    if (movedPiece->name == PieceType::PAWN && (abs(from - to) == 7 || abs(from - to) == 9) && capturedPiece == nullptr) {
        isEnPassant = true;
    }

    if (movedPiece->name == PieceType::PAWN && (to < 8 || to > 55)) {
        isPromotion = true;
    }

    if (isPromotion) {
        promotionPiece = movedPiece->name;
    }
}