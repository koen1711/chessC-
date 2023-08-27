#include "Move.h"
#include "../Setup/Setup.h"

Move::Move(int from, int to, Board* board) : fromSquare(from), toSquare(to)  {
    movedPiece = board->getPieceType(from);
    capturedPiece = board->getPieceType(to);

    // Check if the move is a castle
    if (movedPiece == PieceType::KING && abs(from - to) == 2) {
        isCastle = true;
    } else if (movedPiece == PieceType::PAWN && abs(from - to) == 16) {
        isPawnDoubleMove = true;
    }

    //if (!movedPiece->pFlags->MOVED) {
    //    firstMove = true;
    //} else {
    //    firstMove = false;
    //}

    if (movedPiece == PieceType::PAWN && (abs(from - to) == 7 || abs(from - to) == 9) && capturedPiece == NONE) {
        isEnPassant = true;
        if (board->getPieceColor(from) == ChessColor::COLORWHITE) {
            capturedPiece = board->getPieceType(to - 8);
        } else {
            capturedPiece = board->getPieceType(to + 8);
        }
    }

    if (movedPiece == PieceType::PAWN && (to < 8 || to > 55)) {
        isPromotion = true;
    }

    if (isPromotion) {
        promotionPiece = movedPiece;
    }
}
