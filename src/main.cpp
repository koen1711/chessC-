
#include <raylib.h>
#include <iostream>
#include <chrono>
#include "Board/ViewBoard.h"
#include "Render/Renderer.h"

int moveCount = 0;

void MoveGenerationTest(int depth, Board* board, Renderer* renderer)
{
    if (depth == 0) {
        return;
    }

    std::unordered_map<int, std::vector<Move*>> moveMap = board->moveMap;
    for (const auto& pair : moveMap) {
        auto moves = pair.second;
        if (depth == 1) {
            moveCount += moves.size();
        }
        for (auto move : moves) {

            board->makeMove(move);
            if (depth != 1) {
                board->gameLoop();
            }
            //BeginDrawing();
            //renderer->render();
            //EndDrawing();
            MoveGenerationTest(depth - 1, board, renderer);
            board->undoMove(*move);
        }
    }

}

int main(int argc, char** argv)
{
    int screenWidth = 800;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Chess Game");
    SetTargetFPS(60);

    auto* board = new ViewBoard();
    board->init();
    auto* renderer = new Renderer(board);

    Vector2 mousePosition = {0, 0};
    for (int i = 1; i < 1; i++) {
        // Start the timer
        auto start = std::chrono::high_resolution_clock::now();
        MoveGenerationTest(i, board->board, renderer);
        std::cout << "Depth: " << i << " Move Count: " << moveCount << " Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << "ms" << std::endl;
        moveCount = 0;
        board->board->moveGenerator->GenerateCapturesOnly();
        board->board->gameLoop();
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