#ifndef CHESSC_BOARD_H
#define CHESSC_BOARD_H

#include <vector>
#include "../Move/Move.h"
#include "../MoveGenerator/MoveGenerator.h"
#include "../GameLoop/GameLoop.h"
#include "../Types/Types.h"
#include "../Move/Move.h"
#include <cstdint>
#include <unordered_map>

class Move;

class MoveGenerator;

class GameLoop;

class Board {
public:
    Board();

    GameLoop* gameLoopClass;

    uint64_t controlledSquaresBitboard = 0ULL;
    uint64_t pinnedPieces = 0ULL;
    uint64_t movesThatBlockCheckBitboard = 0ULL;

    uint64_t whitePiecesBitboard = 0ULL;
    uint64_t blackPiecesBitboard = 0ULL;

    uint64_t enPassantLeftBitboard = 0ULL;
    uint64_t enPassantRightBitboard = 0ULL;

    uint64_t pawnsBitboard = 0ULL;
    uint64_t knightsBitboard = 0ULL;
    uint64_t bishopsBitboard = 0ULL;
    uint64_t rooksBitboard = 0ULL;
    uint64_t queensBitboard = 0ULL;
    uint64_t kingsBitboard = 0ULL;
    int whiteKing = 0;
    int blackKing = 0;

    bool inCheck = false;
    bool checkmate = false;

    std::unordered_map<int, std::vector<Move*>> moveMap = {};

    int amountOfChecks = 0;
    bool knightChecked = false;
    ChessColor turn = ChessColor::COLORWHITE;

    void gameLoop() const;

    void makeMove(Move *move);

    void removePiece(int position);

    void promotePawn(int position);

    MoveGenerator* moveGenerator;

    void undoMove(Move* move);

    PieceType getPieceType(int position);
    ChessColor getPieceColor(int position);

    void movePiece(int from, int to);
};


#endif //CHESSC_BOARD_H
