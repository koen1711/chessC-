#ifndef SETTINGS_H
#define SETTINGS_H

#include "raylib.h"

class Settings
{
private:
    /* data */
public:
    Rectangle backButtonPos;
    bool goBack = false;

    Settings(/* args */);
    ~Settings();
    void drawSettingsMenu();
    void mouseLeftButton(Vector2 mousePosition);
};




#endif
