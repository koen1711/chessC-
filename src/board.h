#ifndef BOARD
#define BOARD

#include "setup.h"
#include <any>
#include <map>
#include <string>
#include <memory>
#include <array>
#include "raylib.h"
#include <string>
#include <list>

using namespace std;

class Piece;

class Board
{
private:
    Setup setup;


public:
    std::map<int, std::array<int, 2>> boardPositions;
    std::map<int, Piece*> pieces;
    std::map<Piece*, std::string> piecePositions;
    Board(/* args */);
    ~Board();
    void fenNameToPiece(char fenName, int position);
    void drawBoard();
    void mouseLeftClick(Vector2 mousePoint);
};

#endif