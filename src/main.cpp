#include "raylib.h"
#include "main.h"
#include "Test.h"
#include "pieces.h"
#include "board.h"
#include <iostream>
#include <chrono>
#include <thread>

#define screenWidth 650
#define screenHeight 650


int main(void) {

    Board board;
    Test test;
    std::cout << test.testInclude() << std::endl;
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    

    board.drawBoard();

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
