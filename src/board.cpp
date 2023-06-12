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
#include <list>
#include <algorithm>


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

#define w std::string("white")
#define b std::string("black")

std::string fenNameToColor(char fenName) {
    // if uper case then black
    // if lower case then white
    switch (fenName)
    {
    case 'p':
    case 'r':
    case 'n':
    case 'b':
    case 'q':
    case 'k':
        return w;
        break;
    case 'P':
    case 'R':
    case 'N':
    case 'B':
    case 'Q':
    case 'K':
        return b;
        break;
    }
}

void Board::renderPiece(const std::string& fenName, int piecePositionX, int piecePositionY) {
    piecePositionX = clippingRect.x + piecePositionX;
    piecePositionY = clippingRect.y + piecePositionY;
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

void Board::fenNameToPiece(char fenName, int position) {
    switch (fenName) {
        case 'p':
            pieces[position] = new Pawn(position, w, fenName, *this);
            break;
        case 'P':
            pieces[position] = new Pawn(position, b, fenName, *this);
            break;
        case 'r':
            pieces[position] = new Rook(position, w, fenName, *this);
            break;
        case 'R':
            pieces[position] = new Rook(position, b, fenName, *this);
            break;
        case 'n':
            pieces[position] = new Knight(position, w, fenName, *this);
            break;
        case 'N':
            pieces[position] = new Knight(position, b, fenName, *this);
            break;
        case 'b':
            pieces[position] = new Bishop(position, w, fenName, *this);
            break;
        case 'B':
            pieces[position] = new Bishop(position, b, fenName, *this);
            break;
        case 'q':
            pieces[position] = new Queen(position, w, fenName, *this);
            break;
        case 'Q':
            pieces[position] = new Queen(position, b, fenName, *this);
            break;
        case 'k':
            pieces[position] = new King(position, w, fenName, *this);
            whiteKing = pieces[position];
            break;
        case 'K':
            pieces[position] = new King(position, b, fenName, *this);
            blackKing = pieces[position];
            break;
        default:
            break;
    }
}

Board::Board(){}

Board::~Board(){}

void Board::endGame(std::string state) {
    // loop through pieces and delete them
    for (const auto& pair : pieces) {
        delete pair.second;
    }
    Vector2 vec = MeasureTextEx(GetFontDefault(), "Game Over", 20, 1);
    Vector2 fontPosition;

    fontPosition.x = GetScreenWidth()/2 - MeasureTextEx(GetFontDefault(), "Game Over", 20, 1).x/2;
    fontPosition.y = GetScreenHeight()/2 - 20/2 - 80;
    BeginScissorMode(clippingRect.x, clippingRect.y, clippingRect.width, clippingRect.height);
    BeginDrawing();
    DrawRectangle((GetScreenHeight() / 2) - 150, (GetScreenHeight() / 2) - 100, 300, 200, DARKGRAY);
    DrawTextEx(GetFontDefault(), "Game Over", fontPosition, 20, 1, BLACK);
    // display who won or draw
    fontPosition.x = GetScreenWidth()/2 - MeasureTextEx(GetFontDefault(), state.c_str(), 20, 1).x/2;
    fontPosition.y = GetScreenHeight()/2 - 20/2 - 40;
    DrawTextEx(GetFontDefault(), state.c_str(), fontPosition, 20, 1, BLACK);
    EndDrawing();
    EndScissorMode();
}

bool Board::checkIfPieceProtected(Piece* p) {
    for (const auto& pair : pieces) {
        const auto& i = pair.second;
        if (i == nullptr) {
            continue;
        } else if (i->color == p->color) {
            for (const auto& j : i->getMoves( pieces)) {
                if (j == p->position) {
                    return true;
                }
            }
        }
    }
}


// Game state functions
bool Board::checkForCheck(std::string color) {
    std::vector<int> moves_block_check;
    bool check = false;
    int king_pos = findKing(color);

    if (king_pos == -1) {
        return false;
    }

    for (int i = 0; i < 64; i++) {
        Piece* piece = pieces[i];
        if (!piece) {
            continue;
        }
        if (piece->color != color && !dynamic_cast<King*>(piece)) {
            std::list<int> piece_moves;
            if (dynamic_cast<Pawn*>(piece)) {
                if (piece->color == "white") {
                    if (piece->position - 7 == blackKing->position|| piece->position - 9 == blackKing->position) {
                        check = true;
                        break;
                    }
                } else {
                    if (piece->position + 7 == whiteKing->position || piece->position + 9 == whiteKing->position) {
                        check = true;
                        break;
                    }
                }
            } else {
                piece_moves = piece->getMoves(pieces);
            }
            for (int move : piece_moves) {
                if (move == king_pos) {
                    check = true;
                    moves_block_check.push_back(i);
                    std::vector<int> line_of_attack = getLineOfAttack(i, king_pos);
                    for (int j : line_of_attack) {
                        moves_block_check.push_back(j);
                    }
                }
            }
            moves_block_check.push_back(piece->position);
        }

    }


    if (check) {
        movesThatBlockCheck = moves_block_check;
        return true;
    }

    return false;
}

bool Board::checkForStalemate(std::string color) {
    // Check if the current player's king is in check
    if (checkForCheck(color)) {
        return false;  // The king is in check, not a stalemate
    }

    // Iterate over all pieces of the current player's color
    for (const auto& pair : pieces) {
        Piece* piece = pair.second;
        if (!piece || piece->color != color) {
            continue;
        }

        // Get all possible moves for the current piece
        std::list<int> pieceMoves = piece->getMoves(pieces);

        // Try each move and check if it results in the king being in check
        for (int move : pieceMoves) {
            // Make a copy of the current board state
            std::map<int, Piece*> tempPieces = pieces;
            tempPieces[piece->position] = nullptr;
            tempPieces[move] = piece;
            int oldPos = piece->position;
            piece->position = move;
            // Check if the move puts the king in check
            if (!checkForCheckInPosition(tempPieces, color)) {
                piece->position = oldPos;
                return false;  // The move prevents stalemate, not a stalemate
            }
            piece->position = oldPos;
        }
    }

    return true;  // Stalemate condition reached
}

bool Board::checkForCheckInPosition(std::map<int, Piece*> pieces1, std::string color) {
    int king_pos = findKing(color);
    bool hitking = false;

    if (king_pos == -1) {
        return false;
    }

    for (const auto& pair : pieces1) {
        int i = pair.first;
        Piece*piece = pair.second;
        if (!piece) {
            continue;
        }
        if (piece->color != color) {
            if (dynamic_cast<Pawn*>(piece)) {
                if (piece->color == "white") {
                    if (piece->position - 7 == blackKing->position|| piece->position - 9 == blackKing->position) {
                        hitking = true;
                        break;
                    }
                } else {
                    if (piece->position + 7 == whiteKing->position || piece->position + 9 == whiteKing->position) {
                        hitking = true;
                        break;
                    }
                }
            } else {
                std::list<int> piece_moves = piece->getMoves(pieces1);
                for (int move : piece_moves) {
                    if (move == king_pos) {
                        hitking = true;
                        break;  // No need to check further moves for this piece
                    }
                }
            }
        }
    }

    if (hitking) {
        return true;
    }

    return false;
}

bool Board::checkForCheckmate(std::string color) {
    if (checkForCheck(color)) {
        if (turn == "white") {
            whiteKing->check = true;
        } else {
            blackKing->check = true;
        }
        for (const auto& pair : pieces) {
            const auto& i = pair.second;
            if (i == nullptr || i->color != color) {
                continue;
            }

            std::list<int> piece_moves = i->getMoves(pieces);
            for (int move : piece_moves) {
                std::map<int, Piece*> pieces1 = pieces;
                pieces1[move] = pieces1[i->position];
                pieces1[i->position] = nullptr;
                int oldPos = i->position;
                i->position = move;
                if (!checkForCheckInPosition(pieces1, color)) {
                    i->position = oldPos;
                    return false;
                }
                i->position = oldPos;
            }
        }
        return true;
    }

    return false;
}

// Helper functions
int Board::getPosition(int row, int column) {
    return row * 8 + column;
}

std::vector<int> Board::getAllBlackMoves() {
    std::vector<int> moves;
    for (const auto& pair : pieces) {
        std::list<int> piece_moves;
        const auto& i = pair.second;
        if (i == nullptr || i->color != "black") {
            continue;
        }
        if (i->name == "pawn") {
            piece_moves = { i->position + 7, i->position + 9 };
        } else {
            piece_moves = i->getMoves(pieces);
        }
        for (int move : piece_moves) {
            moves.push_back(move);
        }
    }
    return moves;
}

std::vector<int> Board::getAllWhiteMoves() {
    std::vector<int> moves;
    for (const auto& pair : pieces) {
        std::list<int> piece_moves;
        const auto& i = pair.second;
        if (i == nullptr || i->color != "white") {
            continue;
        }
        if (i->name == "pawn") {
            piece_moves = { i->position + 7, i->position + 9 };
        } else {
            piece_moves = i->getMoves(pieces);
        }
        for (int move : piece_moves) {
            moves.push_back(move);
        }
    }
    return moves;
}

void Board::promotePawn(Piece* pawn) {
    BeginScissorMode(clippingRect.x, clippingRect.y, clippingRect.width, clippingRect.height);
    promoting = true;
    if (pawn->color == "white") {

        BeginDrawing();
            DrawRectangle(getColumn(pawn->position) * 82.5, 0, 83, 4 * 82.5, GRAY);
            renderPiece("queen-white", getColumn(pawn->position) * 82.5, 0);
            renderPiece("rook-white", getColumn(pawn->position) * 82.5, 82.5);
            renderPiece("bishop-white", getColumn(pawn->position) * 82.5, 2 * 82.5);
            renderPiece("knight-white", getColumn(pawn->position) * 82.5, 3 * 82.5);
            // add all the
        EndDrawing();
        // add all the promoting pieces positions
        promotionPosition.insert(std::make_pair("queen-white", getPosition(0, getColumn(pawn->position))));
        promotionPosition.insert(std::make_pair("rook-white", getPosition(1, getColumn(pawn->position))));
        promotionPosition.insert(std::make_pair("bishop-white", getPosition(3, getColumn(pawn->position))));
        promotionPosition.insert(std::make_pair("knight-white", getPosition(4, getColumn(pawn->position))));
    } else {
        BeginDrawing();
            DrawRectangle(getColumn(pawn->position) * 82.5, 650 - (4 * 82.5), 83, 4 * 82.5, GRAY);
            renderPiece("queen-black", getColumn(pawn->position) * 82.5, 650 - 82.5);
            renderPiece("rook-black", getColumn(pawn->position) * 82.5, 650 - (2*82.5));
            renderPiece("bishop-black", getColumn(pawn->position) * 82.5, 650 - (3*82.5));
            renderPiece("knight-black", getColumn(pawn->position) * 82.5, 650 - (4*82.5));

        EndDrawing();
        // add all the promoting pieces positions
        promotionPosition.insert(std::make_pair("queen-black", getPosition(7, getColumn(pawn->position))));
        promotionPosition.insert(std::make_pair("rook-black", getPosition(6, getColumn(pawn->position))));
        promotionPosition.insert(std::make_pair("bishop-black", getPosition(5, getColumn(pawn->position))));
        promotionPosition.insert(std::make_pair("knight-black", getPosition(4, getColumn(pawn->position))));
    }
    EndScissorMode();
}


std::vector<int> Board::getLineOfAttack(int pos1, int pos2) {
    std::vector<int> line_of_attack;
    if (pos1 > pos2) {
        std::swap(pos1, pos2);
    }
    if (pos1 % 8 == pos2 % 8) {
        for (int i = pos1; i <= pos2; i++) {
            line_of_attack.push_back(i);
        }
    } else if (pos1 / 8 == pos2 / 8) {
        for (int i = pos1; i <= pos2; i += 8) {
            line_of_attack.push_back(i);
        }
    } else if (std::abs(pos1 % 8 - pos2 % 8) == std::abs(pos1 / 8 - pos2 / 8)) {
        int step = (pos1 % 8 > pos2 % 8) ? 7 : 9;
        for (int i = pos1; i <= pos2; i += step) {
            line_of_attack.push_back(i);
        }
    }
    return line_of_attack;
}

int Board::getColumn(int pos) {
    // find the passed pos in the rows map
    for (const auto& pair : columns) {
        const auto& i = pair.second;
        if (std::find(i.begin(), i.end(), pos) != i.end()) {
            return pair.first;
        }
    }
}

int Board::getRow(int pos) {
    // find the passed pos in the rows map
    for (const auto& pair : rows) {
        const auto& i = pair.second;
        if (std::find(i.begin(), i.end(), pos) != i.end()) {
            return pair.first;
        }
    }
}

int Board::findKing(std::string color) {
    if (color == w) {
        return whiteKing->position;
    } else {
        return blackKing->position;
    }
    return -1;
}


bool Board::isLegalMove(int from, int to) {
    // Check if the move is valid according to the specific game rules
    // ...
    // Return true if the move is legal, otherwise false
    return true;
}

std::vector<int> Board::getSquaresBetween(int pos1, int pos2) {
    std::vector<int> squares_between;

    if (pos1 > pos2) {
        std::swap(pos1, pos2);
    }

    if (pos1 % 8 == pos2 % 8 || pos1 / 8 == pos2 / 8) {
        for (int i = pos1; i <= pos2; i++) {
            squares_between.push_back(i);
        }
    } else if (std::abs(pos1 % 8 - pos2 % 8) == std::abs(pos1 / 8 - pos2 / 8)) {
        int step = (pos1 % 8 > pos2 % 8) ? 7 : 9;
        for (int i = pos1; i <= pos2; i += step) {
            squares_between.push_back(i);
        }
    } else if ((std::abs(pos1 % 8 - pos2 % 8) == 2 && std::abs(pos1 / 8 - pos2 / 8) == 1) ||
               (std::abs(pos1 % 8 - pos2 % 8) == 1 && std::abs(pos1 / 8 - pos2 / 8) == 2)) {
        squares_between.push_back(pos2);
    } else {
        if (pos1 % 8 == pos2 % 8) {
            int step = (pos1 > pos2) ? -8 : 8;
            for (int i = pos1 + step; i != pos2; i += step) {
                squares_between.push_back(i);
            }
        } else {
            int step = (pos1 > pos2) ? -1 : 1;
            for (int i = pos1 + step; i != pos2; i += step) {
                squares_between.push_back(i);
            }
        }
    }

    return squares_between;
}

void Board::initBoard()
{
    BeginScissorMode(clippingRect.x, clippingRect.y, clippingRect.width, clippingRect.height);
    BeginDrawing();

    ClearBackground(RED);

    // loop 64 times to draw the chess board
    int i = 0;
    int j = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            // draw a square
            // if i is even, draw a white square
            // if i is odd, draw a black square
            if (x % 2 == i)
            {
                DrawRectangle(clippingRect.x + x * 82.5, clippingRect.y + y * 82.5, clippingRect.x + (82.5 * x) + 82.5, clippingRect.y + (82.5 * y) + 82.5, WHITE);
            }
            else
            {
                DrawRectangle(clippingRect.x + x * 82.5, clippingRect.y + y * 82.5, clippingRect.x + (82.5 * x) + 82.5, clippingRect.y + (82.5 * y) + 82.5, DARKBROWN);
            }
            // add the positions of each square to the board
            std::array<int, 2> position = { clippingRect.x + 82.5 * x, clippingRect.y + 82.5 * y };
            boardPositions.insert(std::make_pair(j, position));
            j++;
        }
        if (i == 1)
        {
            i = 0;
        }
        else
        {
            i = 1;
        }
    }

    EndDrawing();
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    Setup setup;

    setup.addFenToSetup(fen);
    fen = "";
    for (auto const& piece : setup.pieces)
    {
        // pieces = map<string, list<int>>, piece.first is the fenName and piece.second is the position array, so loop through the positions and draw the pieces
        for (auto const& pos : piece.second)
        {
            fenNameToPiece(piece.first[0], pos);
        }
    }

    // add all the pieces to the board, and NULL value for empty squares
    BeginDrawing();
    for (auto piece : pieces)
    {
        std::array<int, 2> position;
        try
        {
            position = boardPositions.at(piece.second->position);
        }
        catch (const std::out_of_range& e)
        {
            // Handle the case where the position is not found in the map
            // *this could be due to an incorrect position value or an empty map
            // You might want to add appropriate error handling here
            continue;
        }

        if (piece.second == nullptr)
        {
            continue;
        }

        if (piece.second->name.empty())
        {
            continue;
        }

        const std::string& fenName = std::string(piece.second->name) + "-" + std::string(fenNameToColor(piece.second->fenName));
        renderPiece(fenName, position[0], position[1]);
    }
    EndDrawing();
    EndScissorMode();
}

void Board::drawBoard()
{
    BeginScissorMode(clippingRect.x, clippingRect.y, clippingRect.width, clippingRect.height);
    BeginDrawing();
    
    ClearBackground(RED);

    // loop 64 times to draw the chess board
    int i = 0;
    int j = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            // draw a square
            // if i is even, draw a white square
            // if i is odd, draw a black square
            if (x % 2 == i)
            {
                DrawRectangle(clippingRect.x + x * 82.5, clippingRect.y + y * 82.5, clippingRect.x + (82.5 * x) + 82.5, clippingRect.y + (82.5 * y) + 82.5, WHITE);
            }
            else
            {
                DrawRectangle(clippingRect.x + x * 82.5, clippingRect.y + y * 82.5, clippingRect.x + (82.5 * x) + 82.5, clippingRect.y + (82.5 * y) + 82.5, DARKBROWN);
            }
            if (pieces[j] != nullptr)
            {
                std::array<int, 2> position = boardPositions.at(j);
                const std::string& fenName = std::string(pieces[j]->name) + "-" + std::string(fenNameToColor(pieces[j]->fenName));
                renderPiece(fenName, position[0], position[1]);
            }

            j++;
        }
        if (i == 1)
        {
            i = 0;
        }
        else
        {
            i = 1;
        }
    }

    EndDrawing();
    EndScissorMode();
}

void Board::mouseLeftClick(Vector2 mousePoint) {
    // check which square the mouse is in
    // loop through the boardPositions map and check if the mousePoint is in the square
    int clickedSquare = -1;

    for (auto const& position : boardPositions) {
        if (mousePoint.x >= position.second[0] && mousePoint.x <= position.second[0] + 82.5 && mousePoint.y >= position.second[1] && mousePoint.y <= position.second[1] + 82.5) {
            clickedSquare = position.first;
            break;
        }
    }

    bool p = promoting;
    if (promoting) {
        for (auto const& position : promotionPosition) {
            if (position.second == clickedSquare) {
                if (position.first == "queen-white") {
                    char piece = "q"[0];
                   fenNameToPiece(piece, getPosition(0, getColumn(clickedSquare)));
                } else if (position.first == "rook-white") {
                    char piece = "r"[0];
                    fenNameToPiece(piece, getPosition(0, getColumn(clickedSquare)));
                } else if (position.first == "bishop-white") {
                    char piece = "b"[0];
                    fenNameToPiece(piece, getPosition(0, getColumn(clickedSquare)));
                } else if (position.first == "knight-white") {
                    char piece = "n"[0];
                    fenNameToPiece(piece, getPosition(0, getColumn(clickedSquare)));
                } else if (position.first == "queen-black") {
                    char piece = "Q"[0];
                    fenNameToPiece(piece, getPosition(7, getColumn(clickedSquare)));
                } else if (position.first == "rook-black") {
                    char piece = "R"[0];
                    fenNameToPiece(piece, getPosition(7, getColumn(clickedSquare)));
                } else if (position.first == "bishop-black") {
                    char piece = "B"[0];
                    fenNameToPiece(piece, getPosition(7, getColumn(clickedSquare)));
                } else if (position.first == "knight-black") {
                    char piece = "N"[0];
                    fenNameToPiece(piece, getPosition(7, getColumn(clickedSquare)));
                }
                drawBoard();
                promoting = false;
                promotionPosition.clear();
                whiteKing->check = false;
                blackKing->check = false;
                whiteKingDoNotMove.clear();
                blackKingDoNotMove.clear();
                whiteKingDoNotMove = getAllBlackMoves();
                blackKingDoNotMove = getAllWhiteMoves();
                if (turn == w) {
                    turn = b;
                } else {
                    turn = w;
                }

                potentialMoves.clear();
                selectedPiece = nullptr;
                drawBoard();
                bool checkmate = checkForCheckmate(turn);
                if (checkmate) {
                    endGame("Checkmate");
                    return;
                } else {
                    bool stalemated = checkForStalemate(turn);
                    if (stalemated) {
                        endGame("Stalemate");
                        return;
                    }
                }

                return;
            }
        }

    }

    if (clickedSquare == -1) {
        return;
    }

    // check if the clickedSquare is in the pieces list
    if (std::find(potentialMoves.begin(), potentialMoves.end(), clickedSquare) != potentialMoves.end()) {

        // reset both checks
        whiteKing->check = false;
        blackKing->check = false;
        whiteKingDoNotMove.clear();
        blackKingDoNotMove.clear();
        whiteKingDoNotMove = getAllBlackMoves();
        blackKingDoNotMove = getAllWhiteMoves();
        selectedPiece->makeMove(clickedSquare);
        if (promoting == true && p == false) {
            // check if user clicked in one of promotionPosition
            return;
        }
        if (turn == w) {
            turn = b;
        } else {
            turn = w;
        }

        potentialMoves.clear();
        selectedPiece = nullptr;
        drawBoard();
        bool checkmate = checkForCheckmate(turn);
        if (checkmate) {
            endGame("Checkmate");
            return;
        } else {
            bool stalemated = checkForStalemate(turn);
            if (stalemated) {
                endGame("Stalemate");
                return;
            }
        }

        return;
    }

    if (pieces[clickedSquare] == nullptr) {
        return;
    }

    // check if the piece is the same color as the player
    if (pieces[clickedSquare]->color != turn) {
        return;
    }

    // get the piece and call the move function
    pieces[clickedSquare]->move(pieces);

}
