#include <iostream>
#include "Pieces.h"

Piece::Piece(PieceType name, ChessColor color, int square) {
    std::cout << "Piece constructor called" << std::endl;
    this->name = name;
    this->color = color;
    this->position = square;
}

std::map<int, Move*> Piece::getMoves() {
    return moves;
}

Pawn::Pawn(ChessColor color, int square) : Piece(PieceType::PAWN, color, square) {}
Knight::Knight(ChessColor color, int square) : Piece(PieceType::KNIGHT, color, square) {}
Bishop::Bishop(ChessColor color, int square) : Piece(PieceType::BISHOP, color, square) {}
Rook::Rook(ChessColor color, int square) : Piece(PieceType::ROOK, color, square) {}
Queen::Queen(ChessColor color, int square) : Piece(PieceType::QUEEN, color, square) {}
King::King(ChessColor color, int square) : Piece(PieceType::KING, color, square) {}