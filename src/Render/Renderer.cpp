#include "Renderer.h"

#include <raylib.h>
#include <iostream>

Renderer::Renderer(ViewBoard* board)
{
    this->viewBoard = board;
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
    }

    // draw all the moves that block check
    for (auto piece : viewBoard->board->pinnedPieces) {
        int x = piece->position % 8;
        int y = piece->position / 8;
        DrawRectangle(x * 100, y * 100, 100, 100, BLUE);
    }

    for (const auto& entry : viewBoard->potentialMoves) {
        if (entry== nullptr) {
            continue;
        }
        int x = entry->toSquare % 8;
        int y = entry->toSquare / 8;
        if (entry->capturedPiece != nullptr) {
            DrawTextureEx(captureTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage, WHITE);
        } else {
            DrawTextureEx(potentialMoveTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage, WHITE);
        }
    }
}

void Renderer::drawPieces() {
    float scaleOfImage = ((float)GetScreenWidth() / (float)(8)) / 82;
    for (auto piece : viewBoard->board->pieces) {
        int x = piece->position % 8;
        int y = piece->position / 8;
        PieceType name = piece->name;
        if (piece->color == ChessColor::COLORWHITE) {
            switch (piece->name) {
                case PieceType::KING:
                    DrawTextureEx(whiteKingTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage, WHITE);
                    break;
                case PieceType::QUEEN:
                    DrawTextureEx(whiteQueenTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage,
                                  WHITE);
                    break;
                case PieceType::ROOK:
                    DrawTextureEx(whiteRookTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage, WHITE);
                    break;
                case PieceType::BISHOP:
                    DrawTextureEx(whiteBishopTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage,
                                  WHITE);
                    break;
                case PieceType::KNIGHT:
                    DrawTextureEx(whiteKnightTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage,
                                  WHITE);
                    break;
                case PieceType::PAWN:
                    DrawTextureEx(whitePawnTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage, WHITE);
                    break;
            }
        } else {
            switch (name) {
                case PieceType::KING:
                    DrawTextureEx(blackKingTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage, WHITE);
                    break;
                case PieceType::QUEEN:
                    DrawTextureEx(blackQueenTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage,
                                  WHITE);
                    break;
                case PieceType::ROOK:
                    DrawTextureEx(blackRookTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage, WHITE);
                    break;
                case PieceType::BISHOP:
                    DrawTextureEx(blackBishopTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage,
                                  WHITE);
                    break;
                case PieceType::KNIGHT:
                    DrawTextureEx(blackKnightTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage,
                                  WHITE);
                    break;
                case PieceType::PAWN:
                    DrawTextureEx(blackPawnTexture, {(float) (x * 100), (float) (y * 100)}, 0, scaleOfImage, WHITE);
                    break;
            }
        }
    }
}