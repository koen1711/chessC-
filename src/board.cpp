#include "board.h"
#include "raylib.h"
#include "pieces.h"
#include "setup.h"
#include <string>
#include <any>
#include <type_traits>
#include <iostream>
#include <memory>
#include <map>


#define WhitePawnTexture LoadTexture("assets/white-pawn.png")
#define WhiteRookTexture LoadTexture("assets/white-rook.png")
#define WhiteKnightTexture LoadTexture("assets/white-knight.png")
#define WhiteBishopTexture LoadTexture("assets/white-bishop.png")
#define WhiteQueenTexture LoadTexture("assets/white-queen.png")
#define WhiteKingTexture LoadTexture("assets/white-king.png")
#define BlackPawnTexture LoadTexture("assets/black-pawn.png")
#define BlackRookTexture LoadTexture("assets/black-rook.png")
#define BlackKnightTexture LoadTexture("assets/black-knight.png")
#define BlackBishopTexture LoadTexture("assets/black-bishop.png")
#define BlackQueenTexture LoadTexture("assets/black-queen.png")
#define BlackKingTexture LoadTexture("assets/black-king.png")

using namespace std;

char fenNameToRealName(char fenName) {
    switch (fenName)
    {
    case 'p':
        return 'pawn';
        break;
    case 'r':
        return 'rook';
        break;
    case 'n':
        return 'knight';
        break;
    case 'b':
        return 'bishop';
        break;
    case 'q':
        return 'queen';
        break;
    case 'k':
        return 'king';
        break;
    default:
        return ' ';
        break;
    }
}

void renderPiece(const std::string& fenName, int piecePositionX, int piecePositionY) {
    if (fenName == "pawn-white") // White Pawn
        DrawTexture(WhitePawnTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "rook-white") // White Rook
        DrawTexture(WhiteRookTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "knight-white") // White Knight
        DrawTexture(WhiteKnightTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "bishop-white") // White Bishop
        DrawTexture(WhiteBishopTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "queen-white") // White Queen
        DrawTexture(WhiteQueenTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "king-white") // White King
        DrawTexture(WhiteKingTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "pawn-black") // Black Pawn
        DrawTexture(BlackPawnTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "rook-black") // Black Rook
        DrawTexture(BlackRookTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "knight-black") // Black Knight
        DrawTexture(BlackKnightTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "bishop-black") // Black Bishop
        DrawTexture(BlackBishopTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "queen-black") // Black Queen
        DrawTexture(BlackQueenTexture, piecePositionX, piecePositionY, WHITE);
    else if (fenName == "king-black") // Black King
        DrawTexture(BlackKingTexture, piecePositionX, piecePositionY, WHITE);
}

#define w std::string("white")
#define b std::string("black")

void Board::fenNameToPiece(char fenName, int position) {
    std::cout << "fenName: " << fenName << std::endl;
    switch (fenName) {
        case 'p':
            pieces[position] = new Pawn(position, b);
            break;
        case 'P':
            pieces[position] = new Pawn(position, w);
            break;
        case 'r':
            pieces[position] = new Rook(position, b);
            break;
        case 'R':
            pieces[position] = new Rook(position, w);
            break;
        case 'n':
            pieces[position] = new Knight(position, b);
            break;
        case 'N':
            pieces[position] = new Knight(position, w);
            break;
        case 'b':
            pieces[position] = new Bishop(position, b);
            break;
        case 'B':
            pieces[position] = new Bishop(position, b);
            break;
        case 'q':
            pieces[position] = new Queen(position, b);
            break;
        case 'Q':
            pieces[position] = new Queen(position, b);
            break;
        case 'k':
            pieces[position] = new King(position, b);
            break;
        case 'K':
            pieces[position] = new King(position, b);
            break;
        default:
            break;
    }
}

Board::Board()
{

}

Board::~Board()
{
}

void Board::drawBoard() {
    BeginDrawing();

        ClearBackground(RED);

        // loop 64 times to draw the chess board
        int i = 0;
        int j = 0;
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                // draw a square
                // if i is even, draw a white square
                // if i is odd, draw a black square
                if (x % 2 == i) {
                    DrawRectangle(x * 82.5, y * 82.5, (82.5 * x) + 82.5, (82.5 * y) + 82.5, WHITE);
                } else {
                    DrawRectangle(x * 82.5, y * 82.5, (82.5 * x) + 82.5, (82.5 * y) + 82.5, BLACK);
                }
                // add the positions of each square to the board
                array<int, 2> position = {82.5 * x, 82.5 * y};
                boardPositions.insert(std::make_pair(j, position));
                j++;
            }
            if (i == 1) {
                i = 0;
            } else {
                i = 1;
            }

        }

    EndDrawing();
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    Setup setup;

    setup.addFenToSetup(fen);
    fen = "";
    for (auto const& piece : setup.pieces) {
        // call fenNameToPiece to get the piece class

        int pos = std::stoi(piece.second.c_str());
        //std::cout << "Type: " << std::string(piece.first[1]) << std::endl;
        fenNameToPiece(piece.first[0], pos);
    }

    // add all the pieces to the board, and NULL value for empty squares

    for (auto piece : pieces) {
        BeginDrawing();
        std::cout << "Pos: " << boardPositions.size() << std::endl;
        std::cout << "Asked pos: " << piece.second->position << std::endl;

        std::array<int, 2> position;
        try {
            position = {0, 0};
        } catch (const std::out_of_range& e) {
            // Handle the case where the position is not found in the map
            // This could be due to an incorrect position value or an empty map
            // You might want to add appropriate error handling here
            continue;
        }


        if (piece.second == nullptr) {
            std::cout << "Piece is null" << std::endl;
            continue;
        }



        if (piece.second->name.empty()) {
            std::cout << "Piece: " << typeid(piece.second).name() << std::endl;
            std::cout << "Piece name is empty" << std::endl;
            continue;
        }

        const std::string& fenName = std::string(piece.second->name) + "-" +piece.second->color;

        std::cout << "1 name: " << fenName << std::endl;
        std::cout << "2 Name: " << piece.second->name << "-" << piece.second->color << std::endl;

        renderPiece(fenName, position[0], position[1]);

        EndDrawing();
    }

}

