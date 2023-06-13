#include "raylib.h"
#include "board.h"
#include "mainmenu.h"
#include "pieces.h"
#include "settings.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#define screenWidth 650
#define screenHeight 650

std::vector<Board> board;
std::vector<MainMenu> mainMenu;
std::vector<Settings> settings;
bool inGame = false;


int main(int argc, const char* args[]) {

    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Chess in C++");

    // set trace log level
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    

    Vector2 mousePoint = { 0.0f, 0.0f };
    // if the first argument is true then start the game
    if (argc > 1) {
        if (std::string(args[1]) == "true") {
            inGame = true;
            board.push_back(Board());
            board[0].clippingRect = { 0, 0, 650, 650 };
            board[0].initBoard();
        } else {
            inGame = false;
            mainMenu.push_back(MainMenu());
            mainMenu[0].drawMainMenu();
        }
    } else {
        inGame = false;
        mainMenu.push_back(MainMenu());
        mainMenu[0].drawMainMenu();
    }
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        mousePoint = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // if there is a board in board[0]
            if (board.size() > 0) {
                board[0].mouseLeftClick(mousePoint);
            } else if (mainMenu.size() > 0) {
                // make a main menu
                mainMenu[0].mouseLeftClick(mousePoint);
                if (mainMenu[0].startGame) {
                    // if the start game button was pressed then start the game
                    inGame = true;
                    board.push_back(Board());
                    if (GetScreenHeight() == 650) {
                        board[0].clippingRect = { 0,0,650,650 };
                    } else {
                        board[0].clippingRect = { (GetScreenWidth() - 650) / 2, (GetScreenHeight() - 650) / 2, 650, 650};
                    }
                    board[0].initBoard();
                    // remove the main menu from memory
                    mainMenu.pop_back();
                }
                if (mainMenu[0].openSettings) {
                    // if the settings button was pressed then open the settings menu
                    settings.push_back(Settings());
                    // remove the main menu from memory
                    delete &mainMenu[0];
                    mainMenu = {};
                    settings[0].drawSettingsMenu();

                }
            } else if (settings.size() > 0) {
                BeginDrawing();
                ClearBackground(BLACK);
                EndDrawing();
                // make a settings menu
                settings[0].mouseLeftButton(mousePoint);
                if (settings[0].goBack) {
                    // if the back button was pressed then go back to the main menu
                    mainMenu.push_back(MainMenu());
                    settings = {};
                    mainMenu[0].drawMainMenu();
                } else {
                    settings[0].drawSettingsMenu();
                }

            }
        }


        // check if there was a change in window size
        if (IsWindowResized()) {
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
            if (board.size() > 0) {
                // if there is a board in board[0] then resize it
                board[0].clippingRect = { (GetScreenWidth() - 650) / 2, (GetScreenHeight() - 650) / 2, 650, 650 };
                board[0].drawBoard();
            } else if (mainMenu.size() > 0) {
                // if there is a main menu in mainMenu[0] then resize it
                mainMenu[0].drawMainMenu();
            } else if (settings.size() > 0) {
                // if there is a settings menu in settings[0] then resize it
                settings[0].drawSettingsMenu();
            }
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
