#ifndef CHESSGAME_BOARD_H
#define CHESSGAME_BOARD_H

#include "Pieces/Pieces.h"
#include "GameLoop/GameLoop.h"
#include "MoveGenerator/MoveGenerator.h"
#include "Move/Move.h"
#include "../Render/Renderer.h"
#include "Types/Types.h"
#include "Board/Board.h"

#include <raylib.h>
#include <vector>
#include <map>
#include <list>
#include "Board/Board.h"

class GameLoop;

class ViewBoard {
public:
    Board* board;
    ChessColor turn = ChessColor::COLORWHITE;

    std::vector<Move*> potentialMoves;

    Piece* activePiece = nullptr;

    std::vector<Piece*> drawingBoard;


    ViewBoard();

    void init();

    // GAMEPLAY FUNCTIONS
    void movePiece(Move* move);

    // INTERACTION FUNCTIONS
    void handleClick(Vector2 mousePosition);
};


#endif //CHESSGAME_BOARD_H
