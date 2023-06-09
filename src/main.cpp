#include "raylib.h"
#include "main.h"
#include "board.h"
#include "pieces.h"
#include <iostream>
#include <chrono>
#include <thread>

#define screenWidth 650
#define screenHeight 650


int main(void) {

    Board board;
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Chess in C++");
    // set trace log level
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    

    board.initBoard();
    Vector2 mousePoint = { 0.0f, 0.0f };

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        mousePoint = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            board.mouseLeftClick(mousePoint);
        }
        BeginDrawing();
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
