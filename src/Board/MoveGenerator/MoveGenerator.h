#ifndef CHESSGAME_MOVEGENERATOR_H
#define CHESSGAME_MOVEGENERATOR_H

#include "../Board/Board.h"
#include "../Pieces/Pieces.h"
#include <vector>
#include <bitset>
#include <algorithm>

class Board;
class Piece;
class Move;

class MoveGenerator {
public:
    Board* board;

    explicit MoveGenerator(Board* board);

    // Public methods for generating moves and captures
    void GenerateMoves();
    void GenerateCapturesOnly();

    int moveSize{};

private:
    
    // Private methods for generating specific types of moves and captures
    void GeneratePawnMoves();
    void GenerateKnightMoves();
    void GenerateBishopMoves();
    void GenerateRookMoves();
    void GenerateQueenMoves();
    void GenerateKingMoves();
    void GenerateOnePawnMoves(Piece* piece, int direction, int startingRow, int enPassantRight, int enPassantLeft) const;
    void GenerateOneKnightMoves(Piece* p) const;
    void GenerateOneBishopMoves(Piece* p) const;
    void GenerateOneRookMoves(Piece* p) const;
    void GenerateOneQueenMoves(Piece* p) const;
    void GenerateOneKingMoves(Piece* p) const;

    std::vector<int> GeneratePawnCaptures() const;
    std::vector<int> GenerateKnightCaptures() const;
    std::vector<int> GenerateBishopCaptures() const;
    std::vector<int> GenerateRookCaptures() const;
    std::vector<int> GenerateQueenCaptures() const;
    std::vector<int> GenerateKingCaptures() const;
};

#endif //CHESSGAME_MOVEGENERATOR_H