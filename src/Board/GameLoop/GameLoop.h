#ifndef CHESSC___GAMELOOP_H
#define CHESSC___GAMELOOP_H

#include "../ViewBoard.h"
#include "../Pieces/Pieces.h"
#include "../Board/Board.h"
#include <map>
#include "../Types/Types.h"

class Board;

class Piece;

class GameLoop {
    private:
        Board* board;
        std::vector<Piece*> attackingPieces;
        std::map<Piece*, Directions> pinnedPieces;

        void checkForCheck();
        void getPinnedPieces();
        void generatePotentialMoves();
        void checkForCheckmate();
        bool checkIfPieceProtected(Piece* p);

    public:
        explicit GameLoop(Board* board);
        ~GameLoop();

        void gameLoop();

    static std::vector<int> getPositionsBetween(int startPos, int endPos);
};


#endif //CHESSC___GAMELOOP_H
