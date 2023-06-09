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
#include <set>
#include <vector>

using namespace std;

class Piece;

class Board
{
private:
    Setup setup;
public:
    Piece* whiteKing;
    Piece* blackKing;
    std::vector<int> whiteKingDoNotMove;
    std::vector<int> blackKingDoNotMove;
    std::vector<int> movesThatBlockCheck;
    std::string turn = "white";
    std::map<int, std::array<int, 2>> boardPositions;
    std::map<int, Piece*> pieces;
    std::map<Piece*, std::string> piecePositions;
    std::list<int> potentialMoves;
    Piece* selectedPiece = nullptr;
    std::set<int> leftEdge = {0, 8, 16, 24, 32, 40, 48, 56};
    std::set<int> rightEdge = {7, 15, 23, 31, 39, 47, 55, 63};
    std::set<int> topEdge = {0, 1, 2, 3, 4, 5, 6, 7};
    std::set<int> bottomEdge = {56, 57, 58, 59, 60, 61, 62, 63};
    Board(/* args */);
    ~Board();
    void fenNameToPiece(char fenName, int position);
    void initBoard();
    void drawBoard();
    void mouseLeftClick(Vector2 mousePoint);
    bool checkForCheck(std::string color);
    std::list<int> getAllWhiteMoves();
    std::list<int> getAllBlackMoves();
    void checkControlledSquares(const std::string& color);
    std::vector<int> getSquaresBetween(int pos1, int pos2);
    int findKing(std::string color);
    std::vector<int> getLineOfAttack(int pos1, int pos2);
    bool canAttack(int pos1, int pos2);
    bool isPinned(int pos);
    bool isLegalMove(int, int);
    bool checkForCheckmate(std::string color);
};

#endif