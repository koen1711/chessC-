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
    drawingBoard = board->board;
}


void ViewBoard::movePiece(Move *move) {
    // move the piece
    board->movePiece(move);
    board->gameLoop();
    // update the drawing board
    drawingBoard = board->board;
    // update the turn
    turn = turn == ChessColor::COLORWHITE ? ChessColor::COLORBLACK : ChessColor::COLORWHITE;

}


void ViewBoard::handleClick(Vector2 mousePosition) {
    int x = mousePosition.x / 100;
    int y = mousePosition.y / 100;
    int position = x + y * 8;

    Piece* piece = board->board[position];
    if (piece != nullptr && piece->color == this->turn) {
        if (piece->color != this->turn)
            return;
        this->potentialMoves = piece->moves;
        this->activePiece = piece;
    } else if (this->activePiece != nullptr) {
        // get the move that corresponds to the position
        Move* move = nullptr;
        for (Move* m : potentialMoves) {
            if (m->toSquare == position) {
                move = m;
                break;
            }
        }
        if (move == nullptr)
            return;
        movePiece(move);
        potentialMoves.clear();

    }
}
