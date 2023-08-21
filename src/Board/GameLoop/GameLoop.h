#ifndef CHESSC___GAMELOOP_H
#define CHESSC___GAMELOOP_H

#include "../Board.h"

class Board;

class GameLoop {
    private:
        Board* board;

        void checkForCheck();
        void getPinnedPieces();
        void checkForCheckmate();
    public:
        explicit GameLoop(Board* board);
        ~GameLoop();

        void gameLoop();
};


#endif //CHESSC___GAMELOOP_H
