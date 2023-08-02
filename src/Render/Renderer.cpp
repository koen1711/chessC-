#include "Renderer.h"

#include <raylib.h>
#include <algorithm>

Renderer::Renderer(Board* board)
{
    this->board = board;
}

void Renderer::render()
{
    drawBoard();
    drawPieces();
}

void Renderer::drawBoard() {
    float scaleOfImage = ((float)GetScreenWidth() / (float)(8)) / 82;
    for (int i = 0; i < 64; i++) {
        int x = i % 8;
        int y = i / 8;

        if ((x + y) % 2 == 0) {
            DrawRectangle(x * 100, y * 100, 100, 100, WHITE);
        } else {
            DrawRectangle(x * 100, y * 100, 100, 100, DARKBROWN);
        }


        if (std::find(board->potentialMoves.begin(), board->potentialMoves.end(), i) != board->potentialMoves.end()) {
            if (board->board[i] != nullptr) {
                DrawTextureEx(captureTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
            } else {
                DrawTextureEx(potentialMoveTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
            }
        }
    }

    // draw all the moves that block check
    for (int move : board->movesThatBlockCheck) {
        int x = move % 8;
        int y = move / 8;
        DrawRectangle(x * 100, y * 100, 100, 100, BLUE);
    }
}

void Renderer::drawPieces() {
    float scaleOfImage = ((float)GetScreenWidth() / (float)(8)) / 82;
    for (int i = 0; i < 64; i++) {
        int x = i % 8;
        int y = i / 8;
        if (board->board[i] != nullptr) {
            std::string name = board->board[i]->name;
            switch (board->board[i]->fenName) {
                case 'K':
                    DrawTextureEx(whiteKingTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'Q':
                    DrawTextureEx(whiteQueenTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'R':
                    DrawTextureEx(whiteRookTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'B':
                    DrawTextureEx(whiteBishopTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'N':
                    DrawTextureEx(whiteKnightTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'P':
                    DrawTextureEx(whitePawnTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'k':
                    DrawTextureEx(blackKingTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'q':
                    DrawTextureEx(blackQueenTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'r':
                    DrawTextureEx(blackRookTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'b':
                    DrawTextureEx(blackBishopTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'n':
                    DrawTextureEx(blackKnightTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
                case 'p':
                    DrawTextureEx(blackPawnTexture, { (float)(x * 100), (float)(y * 100) }, 0, scaleOfImage, WHITE);
                    break;
            }
        }
    }
}