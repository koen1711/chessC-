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

using namespace std;

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

Board::Board()
{
}

Board::~Board()
{
}

void Board::endGame() {
    // loop through pieces and delete them
    for (const auto& pair : pieces) {
        delete pair.second;
    }
    Vector2 vec = MeasureTextEx(GetFontDefault(), "Game Over", 20, 1);
    Vector2 fontPosition;

    fontPosition.x = GetScreenWidth()/2 - MeasureTextEx(GetFontDefault(), "Game Over", 20, 1).x/2;
    fontPosition.y = GetScreenHeight()/2 - 20/2 - 80;
    BeginDrawing();
    DrawRectangle((GetScreenHeight() / 2) - 150, (GetScreenHeight() / 2) - 100, 300, 200, DARKGRAY);
    DrawTextEx(GetFontDefault(), "Game Over", fontPosition, 20, 1, BLACK);
    EndDrawing();
}






void Board::checkControlledSquares(const std::string& color) {
    whiteKingDoNotMove.clear();
    blackKingDoNotMove.clear();

    Piece* king;
    if (color == "white") {
        king = whiteKing;
    } else {
        king = blackKing;
    }

    for (const auto& pair : pieces) {
        const auto& i = pair.second;
        if (i == nullptr) {
            continue;
        } else if (i->color != color && typeid(*i) != typeid(King)) {
            for (const auto& j : i->getPotentialMoves()) {
                if (color == "white") {
                    whiteKingDoNotMove.push_back(j);
                } else {
                    blackKingDoNotMove.push_back(j);
                }
            }
        }
    }
}

bool Board::checkIfPieceProtected(Piece* p) {
    for (const auto& pair : pieces) {
        const auto& i = pair.second;
        if (i == nullptr) {
            continue;
        } else if (i->color == p->color) {
            for (const auto& j : i->getPotentialMoves()) {
                if (j == p->position) {
                    return true;
                }
            }
        }
    }
}

bool Board::checkForCheck(std::string color) {
    std::vector<int> moves_block_check;
    std::vector<int> doNotGoKing;
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
            // check if the piece is a pawn
            std::list<int> piece_moves;
            if (dynamic_cast<Pawn*>(piece)) {
                // check if white or black
                if (piece->color == w) {
                    if (piece->position == blackKing->position + 7 || piece->position == blackKing->position + 9) {
                        check = true;
                        piece_moves = {blackKing->position + 7, blackKing->position + 9};
                    }
                } else {
                    if (piece->position == whiteKing->position - 7 || piece->position == whiteKing->position - 9) {
                        check = true;
                        piece_moves = {whiteKing->position - 7, whiteKing->position - 9};
                    }
                }
            } else {
                piece_moves = piece->getMoves();
            }
            for (int move : piece_moves) {
                if (move == king_pos) {
                    check = true;
                    moves_block_check.push_back(i);
                    doNotGoKing.push_back(move);
                    std::vector<int> line_of_attack = getLineOfAttack(i, king_pos);
                    for (int j : line_of_attack) {
                        moves_block_check.push_back(j);
                        doNotGoKing.push_back(j);
                    }
                    break;  // No need to check further moves for this piece
                } else {
                    doNotGoKing.push_back(move);
                }
            }
            moves_block_check.push_back(piece->position);
            // check if piece is the piece who gives the check is protected
            if (checkIfPieceProtected(piece)) {
                doNotGoKing.push_back(piece->position);
            }
        }
    }

    if (check == true) {
        movesThatBlockCheck = moves_block_check;
        if (color == std::string("white")) {
            whiteKingDoNotMove = doNotGoKing;
        } else {
            blackKingDoNotMove = doNotGoKing;
        }

        return true;
    }

    return false;
}

bool Board::checkForCheckInPosition(std::map<int, Piece*> pieces1, std::string color) {
    std::vector<int> moves_block_check;
    std::vector<int> doNotGoKing;
    int king_pos = findKing(color);
    bool hitking = false;

    if (king_pos == -1) {
        return false;
    }

    for (int i = 0; i < 64; i++) {
        Piece* piece = pieces1[i];
        if (!piece) {
            continue;
        }
        if (piece->color != color && !dynamic_cast<King*>(piece)) {
            std::list<int> piece_moves = piece->getMoves();
            for (int move : piece_moves) {
                if (move == king_pos) {
                    hitking = true;
                    std::vector<int> line_of_attack = getLineOfAttack(i, king_pos);
                    for (int j : line_of_attack) {
                        moves_block_check.push_back(j);
                    }
                    break;  // No need to check further moves for this piece
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
        // if check loop through all friendly pieces can block the check
        for (const auto& pair : pieces) {
            const auto& i = pair.second;
            if (i == nullptr) {
                continue;
            } else if (i->color == color) {
                std::list<int> piece_moves = i->getMoves();
                for (int move : piece_moves) {
                    // now try to make the move and parse that position to the checkForCheckInPosition function
                    std::map<int, Piece*> pieces1 = pieces;
                    pieces1[move] = pieces1[i->position];
                    pieces1[i->position] = nullptr;
                    if (!checkForCheckInPosition(pieces1, color)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool Board::canAttack(int pos1, int pos2) {
    Piece* piece = pieces[pos1];
    if (!piece) {
        return false;
    }
    std::list<int> potential_moves1 = piece->getPotentialMoves();
    std::vector<int> potential_moves(potential_moves1.begin(), potential_moves1.end());

    return std::find(potential_moves.begin(), potential_moves.end(), pos2) != potential_moves.end();
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

int Board::findKing(std::string color) {
    if (color == w) {
        return whiteKing->position;
    } else {
        return blackKing->position;
    }
    return -1;
}

bool Board::isPinned(int pos) {
    Piece* piece = pieces[pos];
    if (!piece) {
        return false;
    }
    std::string color = piece->color;
    int king_pos = findKing(color);
    if (king_pos == -1) {
        return false;
    }
    if (canAttack(pos, king_pos)) {
        std::vector<int> line_of_attack = getLineOfAttack(pos, king_pos);
        for (int i : line_of_attack) {
            if (i != pos && pieces[i]) {
                return true;
            }
        }
    }
    return false;
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



void Board::initBoard() {
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
                    DrawRectangle(x * 82.5, y * 82.5, (82.5 * x) + 82.5, (82.5 * y) + 82.5, DARKBROWN);
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

        // pieces = map<string, list<int>>, piece.first is the fenName and piece.second is the position array, so loop through the positions and draw the pieces
        for (auto const& pos : piece.second) {
            fenNameToPiece(piece.first[0], pos);

        }


    }

    // add all the pieces to the board, and NULL value for empty squares
    BeginDrawing();
    for (auto piece : pieces) {


        std::array<int, 2> position;
        try {
            position = boardPositions.at(piece.second->position);
        } catch (const std::out_of_range& e) {
            // Handle the case where the position is not found in the map
            // *this could be due to an incorrect position value or an empty map
            // You might want to add appropriate error handling here
            continue;
        }


        if (piece.second == nullptr) {
            continue;
        }



        if (piece.second->name.empty()) {
            continue;
        }

        const std::string& fenName = std::string(piece.second->name) + "-" + std::string(fenNameToColor(piece.second->fenName));
        renderPiece(fenName, position[0], position[1]);


    }
    EndDrawing();
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
                    DrawRectangle(x * 82.5, y * 82.5, (82.5 * x) + 82.5, (82.5 * y) + 82.5, DARKBROWN);
                }
                if (pieces[j] != nullptr) {
                    std::array<int, 2> position = boardPositions.at(j);
                    const std::string& fenName = std::string(pieces[j]->name) + "-" + std::string(fenNameToColor(pieces[j]->fenName));
                    renderPiece(fenName, position[0], position[1]);
                }

                j++;
            }
            if (i == 1) {
                i = 0;
            } else {
                i = 1;
            }

        }
    EndDrawing();
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

    if (clickedSquare == -1) {
        return;
    }

    // check if the clickedSquare is in the pieces list
    if (std::find(potentialMoves.begin(), potentialMoves.end(), clickedSquare) != potentialMoves.end()) {
        // reset both checks
        whiteKing->check = false;
        blackKing->check = false;
        selectedPiece->makeMove(clickedSquare);
        if (turn == w) {
            turn = b;
        } else {
            turn = w;
        }
        bool check = checkForCheck(turn);
        std::cout << "check: " << check << std::endl;
        if (turn == "white") {
            whiteKing->check = check;
        } else {
            blackKing->check = check;
        }
        potentialMoves.clear();
        selectedPiece = nullptr;
        drawBoard();
        bool checkmate = checkForCheckmate(turn);
        if (checkmate) {
            endGame();
            return;
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
    pieces[clickedSquare]->move();

}
