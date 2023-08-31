#ifndef CHESSC___GAMELOOP_H
#define CHESSC___GAMELOOP_H

#include "../ViewBoard.h"
#include "../Pieces/Pieces.h"
#include "../Board/Board.h"
#include <unordered_map>
#include "../Types/Types.h"
#include <cstdint>

class Board;

class Piece;

class GameLoop {
    private:
        Board* board;
        uint64_t attackingPieces;
        std::unordered_map<int, Directions> pinnedPieces;

        void checkForCheck();
        void getPinnedPieces();
        void generatePotentialMoves();
        void checkForCheckmate();
        bool checkIfPieceProtected(int p);

    public:
        explicit GameLoop(Board* board);
        ~GameLoop();

        void gameLoop();

    static uint64_t getPositionsBetween(int startPos, int endPos);
};


#endif //CHESSC___GAMELOOP_H
