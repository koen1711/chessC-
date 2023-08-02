#ifndef CHESSGAME_BOARD_H
#define CHESSGAME_BOARD_H

#include "../Pieces/Pieces.h"
#include "../GameLoop/GameLoop.h"
#include "MoveGenerator/MoveGenerator.h"
#include "Move/Move.h"
#include "../Render/Renderer.h"

#include <raylib.h>
#include <vector>
#include <map>



class MoveGenerator;

class Piece;

class GameLoop;

class Move;

class Board {
public:
    MoveGenerator* moveGeneratorClass;
    GameLoop* gameLoopClass;

    bool inCheck = false;
    std::string turn = "white";

    Piece* activePiece = nullptr;
    Piece* whiteKing;
    Piece* blackKing;

    std::vector<int> movesThatBlockCheck;

    // Create a 2D array of board positions
    int boardArray[8][8] = {
            {0, 1, 2, 3, 4, 5, 6, 7},
            {8, 9, 10, 11, 12, 13, 14, 15,},
            {16, 17, 18, 19, 20, 21, 22, 23,},
            {24, 25, 26, 27, 28, 29, 30, 31,},
            {32, 33, 34, 35, 36, 37, 38, 39,},
            {40, 41, 42, 43, 44, 45, 46, 47,},
            {48, 49, 50, 51, 52, 53, 54, 55,},
            {56, 57, 58, 59, 60, 61, 62, 63,}
    };
    std::map<int, Piece*> board;

    std::map<int, Move*> potentialMoves;
    std::vector<int> controlledSquares;

    bool whiteRightCastle = true;
    bool whiteLeftCastle = true;
    bool blackRightCastle = true;
    bool blackLeftCastle = true;

    std::map<std::string, std::vector<Piece*>> pawns;
    std::map<std::string, std::vector<Piece*>> knights;
    std::map<std::string, std::vector<Piece*>> bishops;
    std::map<std::string, std::vector<Piece*>> rooks;
    std::map<std::string, std::vector<Piece*>> queens;
    std::map<std::string, std::vector<Piece*>> kings;

    Board();

    void init();

    // GAMEPLAY FUNCTIONS
    void movePiece(const Move& move);
    void undoMove(const Move& move);
    void removePiece(int position);
    void promotePawn(Piece* pawn);
    void gameLoop();

    // INTERACTION FUNCTIONS
    void handleClick(Vector2 mousePosition);


};


#endif //CHESSGAME_BOARD_H
