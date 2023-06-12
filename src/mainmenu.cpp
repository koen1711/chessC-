#include "mainmenu.h"
#include "raylib.h"
#include <iostream>

MainMenu::MainMenu() {

}

void MainMenu::drawMainMenu() {
    // draw the main menu
    BeginDrawing();
    // position all the text
    Vector2 titlePos = { 0.0f, 0.0f };
    Vector2 playPos = { 0.0f, 0.0f };
    Vector2 quitPos = { 0.0f, 0.0f };
    Vector2 settingsPos = { 0.0f, 0.0f };
    titlePos.x = (GetScreenWidth() / 2) - (MeasureText("Chess in C++", 50) / 2);
    titlePos.y = (GetScreenHeight() / 4) - 100;
    playPos.x = (GetScreenWidth() / 2) - (MeasureText("Play", 50) / 2);
    playPos.y = (GetScreenHeight() / 2) - 50;
    quitPos.x = (GetScreenWidth() / 2) - (MeasureText("Quit", 50) / 2);
    quitPos.y = (GetScreenHeight() / 2) + 50;
    settingsPos.x = (GetScreenWidth() / 2) - (MeasureText("Settings", 50) / 2);
    settingsPos.y = (GetScreenHeight() / 2) + 150;
    // draw the text
    // draw a rect around the text
    DrawText("Chess in C++", titlePos.x, titlePos.y, 50, WHITE);
    DrawRectangleLines(titlePos.x - 10, titlePos.y, MeasureText("Chess in C++", 50) + 20, 50, WHITE);
    DrawText("Play", playPos.x, playPos.y, 50, WHITE);
    DrawRectangleLines(playPos.x - 10, playPos.y, MeasureText("Play", 50) + 20, 50, WHITE);
    DrawText("Quit", quitPos.x, quitPos.y, 50, WHITE);
    DrawRectangleLines(quitPos.x - 10, quitPos.y, MeasureText("Quit", 50) + 20, 50, WHITE);
    DrawText("Settings", settingsPos.x, settingsPos.y, 50, WHITE);
    DrawRectangleLines(settingsPos.x - 10, settingsPos.y, MeasureText("Settings", 50) + 20, 50, WHITE);
    EndDrawing();
    titlePosRec = { titlePos.x, titlePos.y, MeasureText("Chess in C++", 50), 50 };
    playPosRec = { playPos.x - 10, playPos.y, MeasureText("Play", 50) + 20, 50 };
    quitPosRec = { quitPos.x - 10, quitPos.y, MeasureText("Quit", 50) + 20, 50 };
    settingsPosRec = { settingsPos.x - 10, settingsPos.y, MeasureText("Settings", 50) + 20, 50 };
}

void MainMenu::mouseLeftClick(Vector2 mousePoint) {
    // check if the mouse is in the play button
    if (CheckCollisionPointRec(mousePoint, playPosRec)) {
        startGame = true;
    } else if (CheckCollisionPointRec(mousePoint, quitPosRec)) {
        CloseWindow();
    } else if (CheckCollisionPointRec(mousePoint, settingsPosRec)) {
        openSettings = true;
    }
}