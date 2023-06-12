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

class Piece;

class Board
{
private:
    Setup setup;
public:
    Piece* whiteKing;
    Piece* blackKing;
    Rectangle clippingRect;
    std::vector<int> whiteKingDoNotMove;
    std::vector<int> blackKingDoNotMove;
    std::vector<int> movesThatBlockCheck;
    bool promoting = false;
    std::map<std::string, int> promotionPosition;
    bool blackKingMoved = false;
    bool whiteKingMoved = false;
    bool whiteRightRookMoved = false;
    bool whiteLeftRookMoved = false;
    bool blackRightRookMoved = false;
    bool blackLeftRookMoved = false;
    std::string turn = "white";
    std::map<int, std::array<int, 2>> boardPositions;
    std::map<int, Piece*> pieces;
    bool checkForCheckInPosition(std::map<int, Piece*> pieces1, std::string color);
    void endGame(std::string state);
    bool checkIfPieceProtected(Piece* p);
    std::map<Piece*, std::string> piecePositions;
    std::list<int> potentialMoves;
    Piece* selectedPiece = nullptr;
    std::set<int> leftEdge = {0, 8, 16, 24, 32, 40, 48, 56};
    std::set<int> rightEdge = {7, 15, 23, 31, 39, 47, 55, 63};
    std::set<int> topEdge = {0, 1, 2, 3, 4, 5, 6, 7};
    std::set<int> bottomEdge = {56, 57, 58, 59, 60, 61, 62, 63};
    std::map<int, std::vector<int>> rows = { {0, {0, 1, 2, 3, 4, 5, 6, 7} }, {1, {8, 9, 10, 11, 12, 13, 14, 15} }, {2, {16, 17, 18, 19, 20, 21, 22, 23} }, {3, {24, 25, 26, 27, 28, 29, 30, 31} }, {4, {32, 33, 34, 35, 36, 37, 38, 39} }, {5, {40, 41, 42, 43, 44, 45, 46, 47} }, {6, {48, 49, 50, 51, 52, 53, 54, 55} }, {7, {56, 57, 58, 59, 60, 61, 62, 63} } };
    std::map<int, std::vector<int>> columns = { {0, {0, 8, 16, 24, 32, 40, 48, 56} }, {1, {1, 9, 17, 25, 33, 41, 49, 57} }, {2, {2, 10, 18, 26, 34, 42, 50, 58} }, {3, {3, 11, 19, 27, 35, 43, 51, 59} }, {4, {4, 12, 20, 28, 36, 44, 52, 60} }, {5, {5, 13, 21, 29, 37, 45, 53, 61} }, {6, {6, 14, 22, 30, 38, 46, 54, 62} }, {7, {7, 15, 23, 31, 39, 47, 55, 63} } };
    Board(/* args */);
    ~Board();
    void fenNameToPiece(char fenName, int position);
    void initBoard();
    void drawBoard();
    void mouseLeftClick(Vector2 mousePoint);
    bool checkForCheck(std::string color);
    std::vector<int> getAllWhiteMoves();
    bool checkForStalemate(std::string color);
    std::vector<int> getAllBlackMoves();
    std::vector<int> getSquaresBetween(int pos1, int pos2);
    int findKing(std::string color);
    std::vector<int> getLineOfAttack(int pos1, int pos2);
    bool isLegalMove(int, int);
    bool checkForCheckmate(std::string color);
    int getColumn(int pos);
    int getRow(int pos);
    int getPosition(int row, int column);
    void promotePawn(Piece* pawn);
    void renderPiece(const std::string& fenName, int piecePositionX, int piecePositionY);
};

#endif