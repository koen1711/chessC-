#include "settings.h"
#include "raylib.h"

Settings::Settings(/* args */)
{
}

Settings::~Settings()
{
}

void Settings::drawSettingsMenu() {
    Vector2 settingsPos = { 0.0f, 0.0f };
    Vector2 backButton = { 0.0f, 0.0f };
    settingsPos.x = (GetScreenWidth() / 2) - (MeasureText("Settings", 50) / 2);
    settingsPos.y = (GetScreenHeight() / 4) - 100;
    backButton.x = (GetScreenWidth() / 2) - (MeasureText("Back", 50) / 2);
    backButton.y = (GetScreenHeight() / 4) + GetScreenHeight() / 2;

    BeginDrawing();
    ClearBackground(BLACK);


    DrawText("Settings", settingsPos.x, settingsPos.y, 50, WHITE);
    DrawRectangleLines(settingsPos.x - 10, settingsPos.y, MeasureText("Settings", 50) + 20, 50, WHITE);

    DrawText("Back", backButton.x, backButton.y, 50, WHITE);
    DrawRectangleLines(backButton.x - 10, backButton.y, MeasureText("Back", 50) + 20, 50, WHITE);

    EndDrawing();
    backButtonPos = { backButton.x - 10, backButton.y, MeasureText("Back", 50) + 20, 50 };
}

void Settings::mouseLeftButton(Vector2 mousePosition) {
    if (CheckCollisionPointRec(mousePosition, backButtonPos)) {
        goBack = true;
    }
}