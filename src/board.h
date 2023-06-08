#ifndef BOARD
#define BOARD

#include "setup.h"
#include "pieces.h"
#include <any>
#include <map>
#include <string>
#include <memory>
#include <array>

using namespace std;

class Board
{
private:
    Setup setup;


public:
    map<int, array<int, 2>> boardPositions;
    map<int, Piece*> pieces;
    map<any, string> piecePositions;
    Board(/* args */);
    ~Board();
    void fenNameToPiece(char fenName, int position);
    void drawBoard();
};

#endif