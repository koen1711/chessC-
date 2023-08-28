#include "Board/Board.h"
#include "Setup/Setup.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

#define COLOR_WHITE (Color){255, 255, 255, 255}
#define COLOR_BLACK DARKBROWN

ViewBoard::ViewBoard() {
    board = new Board();
    this->init();
}

void ViewBoard::init()
{
}


void ViewBoard::movePiece(Move *move) {
    // move the piece
    board->makeMove(move);
    turn = turn == ChessColor::COLORWHITE ? ChessColor::COLORBLACK : ChessColor::COLORWHITE;
    board->gameLoop();

}


void ViewBoard::handleClick(Vector2 mousePosition) {
    int x = mousePosition.x / 100;
    int y = mousePosition.y / 100;
    int position = x + y * 8;

    if (board->getPieceType(position) != NONE && board->getPieceColor(position) == turn) {
        this->potentialMoves = board->moveMap.at(position);
        this->activePiece = position;
    } else if (this->activePiece != -1 ) {
        // get the move that corresponds to the position
        if (std::find_if(potentialMoves.begin(), potentialMoves.end(), [position](Move* move) { return move->toSquare == position; }) != potentialMoves.end()) {
            // move the piece
            Move* move = new Move(activePiece, position, board);
            movePiece(move);
            potentialMoves.clear();
            activePiece = -1;
        }

    }
}
