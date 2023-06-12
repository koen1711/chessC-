#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"

class MainMenu {
    private:
    public:
        // VARIABLES //
        Rectangle titlePosRec;
        Rectangle playPosRec;
        Rectangle quitPosRec;
        Rectangle settingsPosRec;
        bool startGame = false;
        bool openSettings = false;

        // FUNCTIONS //
        MainMenu();
        void drawMainMenu();
        void mouseLeftClick(Vector2 mousePoint);
};

#endif