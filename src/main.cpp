
#include <raylib.h>
#include <iostream>
#include <chrono>
#include "Board/Board.h"
#include "Render/Renderer.h"

int moveCount = 0;

void MoveGenerationTest(int depth, Board* board, Renderer* renderer)
{
    if (depth == 0) {
        return;
    }

    std::map<int, Piece*> pieces = board->board;
    std::string currentPlayerColor = board->turn == "white" ? "white" : "black";

    for (const auto& pair : pieces) {
        Piece* piece = pair.second;

        if (piece == nullptr)
            continue;
        if (piece->color == currentPlayerColor) {
            std::map<int, Move*> moves = piece->getMoves();
            moveCount += moves.size();
            for (std::pair move : moves) {
                board->movePiece(*move.second);
                //BeginDrawing();
                //renderer->render();
                //EndDrawing();
                MoveGenerationTest(depth - 1, board, renderer);
                board->undoMove(*move.second);
            }
        }
    }
}

int main(int argc, char** argv)
{
    int screenWidth = 800;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Chess Game");
    SetTargetFPS(60);

    auto* board = new Board();
    board->init();
    auto* renderer = new Renderer(board);

    Vector2 mousePosition = {0, 0};
    for (int i = 1; i < 4; i++) {
        // Start the timer
        auto start = std::chrono::high_resolution_clock::now();
        MoveGenerationTest(i, board, renderer);
        std::cout << "Depth: " << i << " Move Count: " << moveCount << " Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms" << std::endl;
        moveCount = 0;
    }

    while (!WindowShouldClose())
    {
        mousePosition = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            board->handleClick(mousePosition);
        }

        BeginDrawing();
            renderer->render();
        EndDrawing();
    }

    return 0;
}