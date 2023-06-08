#ifndef PIECES_H
#define PIECES_H

#include "raylib.h"
#include <string>
#include <list>
#include "board.h"

class Board; // Forward declaration of the Board class

class Piece
{
public:
    char fenName;
    std::string color;
    std::string name;
    int position;
    Board& board; // Reference to the Board object

    Piece(int position, std::string color, char fenName, Board& board);
    virtual ~Piece();
    void drawMoves(std::list<int> moves);
    virtual void move();
    virtual std::list<int> getMoves() = 0;
};

class Pawn : public Piece
{
public:
    Pawn(int position, std::string color, char fenName, Board& board);
    ~Pawn();
    std::list<int> getMoves() override;
};

class Rook : public Piece
{
public:
    Rook(int position, std::string color, char fenName, Board& board);
    ~Rook();
    std::list<int> getMoves() override;
};

class Knight : public Piece
{
public:
    Knight(int position, std::string color, char fenName, Board& board);
    ~Knight();
    std::list<int> getMoves() override;
};

class Bishop : public Piece
{
public:
    Bishop(int position, std::string color, char fenName, Board& board);
    ~Bishop();
    std::list<int> getMoves() override;
};

class Queen : public Piece
{
public:
    Queen(int position, std::string color, char fenName, Board& board);
    ~Queen();
    std::list<int> getMoves() override;
};

class King : public Piece
{
public:
    King(int position, std::string color, char fenName, Board& board);
    ~King();
    std::list<int> getMoves() override;
};

#endif // PIECES_H
