#ifndef CHESSGAME_MOVEGENERATOR_H
#define CHESSGAME_MOVEGENERATOR_H

#include "../Board/Board.h"
#include "../Pieces/Pieces.h"
#include <vector>
#include <bitset>
#include <algorithm>
#include <cstdint>

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
    void GenerateOnePawnMoves(int position, int direction, int startingRow, int enPassantRight, int enPassantLeft) const;
    void GenerateOneKnightMoves(int position) const;
    void GenerateOneBishopMoves(int position) const;
    void GenerateOneRookMoves(int position) const;
    void GenerateOneQueenMoves(int position) const;
    void GenerateOneKingMoves(int position) const;

    uint64_t GeneratePawnCaptures() const;
    uint64_t GenerateKnightCaptures() const;
    uint64_t GenerateBishopCaptures() const;
    uint64_t GenerateRookCaptures() const;
    uint64_t GenerateQueenCaptures() const;
    uint64_t GenerateKingCaptures() const;
};

#endif //CHESSGAME_MOVEGENERATOR_H