#ifndef PIECES
#define PIECES

#include "raylib.h"
#include <string>

class Piece
{
private:

public:
    std::string color;
    std::string name;
    int position;
    Piece();
    ~Piece();

};

class Pawn : public Piece
{
private:

public:
    Pawn() = default;
    Pawn(int position, std::string color);
    ~Pawn();
    int *getMoves();

};

class Rook : public Piece
{
private:

public:
    Rook() = default;
    Rook(int position, std::string color);
    ~Rook();
    int *getMoves();
    
};

class Knight : public Piece
{
private:
    
public:
    Knight() = default;
    Knight(int position, std::string color);
    ~Knight();
    int *getMoves();
    
};

class Bishop : public Piece
{
private:
    
public:
    Bishop() = default;
    Bishop(int position, std::string color);
    ~Bishop();
    int *getMoves();
    
};

class Queen : public Piece
{
private:
    
public:
    Queen() = default;
    Queen(int position, std::string color);
    ~Queen();
    int *getMoves();
    
};

class King : public Piece
{
private:
    
public:
    King() = default;
    King(int position, std::string color);
    ~King();
    int *getMoves();
    
};



#endif