#ifndef CHESSGAME_MOVEGENERATOR_H
#define CHESSGAME_MOVEGENERATOR_H

#include "../Board.h"
#include "../Pieces/Pieces.h"
#include <vector>
#include <bitset>
#include <algorithm>

class Board;

class Piece;

class Move;

class MoveGenerator {
public:
    // Constructor, initialization, and other public methods
    MoveGenerator(Board* board);

    void GenerateMoves();

    int moveSize;
    std::vector<Move*> moves;
private:
    Board* board;


    // Private methods
    void GeneratePawnMoves();
    void GenerateKnightMoves();
    void GenerateBishopMoves();
    void GenerateRookMoves();
    void GenerateQueenMoves();
    void GenerateKingMoves();
};




#endif //CHESSGAME_MOVEGENERATOR_H
