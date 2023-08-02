#ifndef CHESSGAME_RENDERER_H
#define CHESSGAME_RENDERER_H

#include "../Board/Board.h"
#include <raylib.h>

class Board;

class Renderer {
public:
    Renderer(Board* board);
    ~Renderer();

    void render();
private:
    Board* board;
    Texture captureTexture = LoadTexture("assets/selection.png");
    Texture potentialMoveTexture = LoadTexture("assets/potential-move.png");
    Texture whiteKingTexture = LoadTexture("assets/white-king.png");
    Texture whiteQueenTexture = LoadTexture("assets/white-queen.png");
    Texture whiteRookTexture = LoadTexture("assets/white-rook.png");
    Texture whiteBishopTexture = LoadTexture("assets/white-bishop.png");
    Texture whiteKnightTexture = LoadTexture("assets/white-knight.png");
    Texture whitePawnTexture = LoadTexture("assets/white-pawn.png");
    Texture blackKingTexture = LoadTexture("assets/black-king.png");
    Texture blackQueenTexture = LoadTexture("assets/black-queen.png");
    Texture blackRookTexture = LoadTexture("assets/black-rook.png");
    Texture blackBishopTexture = LoadTexture("assets/black-bishop.png");
    Texture blackKnightTexture = LoadTexture("assets/black-knight.png");
    Texture blackPawnTexture = LoadTexture("assets/black-pawn.png");

    void drawBoard();
    void drawPieces();
};


#endif //CHESSGAME_RENDERER_H
