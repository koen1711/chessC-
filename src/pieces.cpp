#include "pieces.h"
#include "raylib.h"
#include <iostream>
#include <string>


#define queen std::string("queen")
#define king std::string("king")
#define rook std::string("rook")
#define bishop std::string("bishop")
#define knight std::string("knight")
#define pawn std::string("pawn")

using namespace std;

Piece::Piece()
{
}

Piece::~Piece()
{
}

int* Pawn::getMoves()
{
    int* moves = new int[1, 0];
    return moves;
}

Pawn::Pawn(int position, std::string color) : Piece()
{
    name = pawn;
    position = position;
    color = color;
    std::cout << "Pawn created" << std::endl;
    std::cout << "Position: " << position << std::endl;
}

Pawn::~Pawn()
{

}

Rook::Rook(int position, std::string color) : Piece()
{
    name = rook;
    position = position;
    color = color;
    std::cout << "Rook created" << std::endl;
    std::cout << "Position: " << position << std::endl;
}

Rook::~Rook()
{

}

Knight::Knight(int position, std::string color) : Piece()
{
    name = knight;
    position = position;
    color = color;
    std::cout << "Knight created" << std::endl;
    std::cout << "Position: " << position << std::endl;
}

Knight::~Knight()
{

}

Bishop::Bishop(int position, std::string color) : Piece()
{
    name = bishop;
    position = position;
    color = color;
    std::cout << "Bishop created" << std::endl;
    std::cout << "Position: " << position << std::endl;
}

Bishop::~Bishop()
{

}

Queen::Queen(int position, std::string color) : Piece()
{
    name = queen;
    position = position;
    color = color;
    std::cout << "Queen created" << std::endl;
    std::cout << "Position: " << position << std::endl;
}

Queen::~Queen()
{

}

King::King(int position, std::string color) : Piece()
{
    name = king;
    position = position;
    color = color;
    std::cout << "King created" << std::endl;
    std::cout << "Position: " << position << std::endl;
}

King::~King()
{

}