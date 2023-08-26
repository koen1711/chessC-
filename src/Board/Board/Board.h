#ifndef CHESSC_BOARD_H
#define CHESSC_BOARD_H

#include <vector>
#include "../Move/Move.h"
#include "../MoveGenerator/MoveGenerator.h"
#include "../GameLoop/GameLoop.h"
#include "../Types/Types.h"
#include "../Move/Move.h"
#include <map>

class Move;

class MoveGenerator;

class GameLoop;

class Board {
public:
    Board();

    GameLoop* gameLoopClass;

    std::vector<int> controlledSquares;
    std::vector<Piece*> board;
    std::vector<Piece*> pinnedPieces;
    std::vector<int> movesThatBlockCheck;

    std::vector<Piece*> pieces;


    std::map<ChessColor, std::vector<int>> pawns = {std::make_pair(ChessColor::COLORWHITE, std::vector<int>()), std::make_pair(ChessColor::COLORBLACK, std::vector<int>())};
    std::map<ChessColor, std::vector<int>> knights = {std::make_pair(ChessColor::COLORWHITE, std::vector<int>()), std::make_pair(ChessColor::COLORBLACK, std::vector<int>())};
    std::map<ChessColor, std::vector<int>> bishops = {std::make_pair(ChessColor::COLORWHITE, std::vector<int>()), std::make_pair(ChessColor::COLORBLACK, std::vector<int>())};
    std::map<ChessColor, std::vector<int>> rooks = {std::make_pair(ChessColor::COLORWHITE, std::vector<int>()), std::make_pair(ChessColor::COLORBLACK, std::vector<int>())};
    std::map<ChessColor, std::vector<int>> queens = {std::make_pair(ChessColor::COLORWHITE, std::vector<int>()), std::make_pair(ChessColor::COLORBLACK, std::vector<int>())};
    std::map<ChessColor, int> kings = {std::make_pair(ChessColor::COLORWHITE, 0), std::make_pair(ChessColor::COLORBLACK, 0)};

    bool inCheck = false;
    bool checkmate = false;

    int amountOfChecks = 0;
    bool knightChecked = false;
    ChessColor turn = ChessColor::COLORWHITE;

    void gameLoop() const;

    void movePiece(Move *move);

    void removePiece(int position);

    void promotePawn(Piece *pawn);

    MoveGenerator* moveGenerator;

    void undoMove(const Move &move);
};


#endif //CHESSC_BOARD_H
