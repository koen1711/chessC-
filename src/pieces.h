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
    bool check = false;
    bool pinned = false;
    char fenName;
    std::string color;
    std::string name;
    int position;
    Board& board; // Reference to the Board object

    Piece(int position, std::string color, char fenName, Board& board);
    virtual ~Piece();
    virtual void makeMove(int position) = 0;
    void drawMoves(std::list<int> moves);
    virtual void move();
    virtual std::list<int> getMoves() = 0;
    virtual std::list<int> getPotentialMoves();
    std::list<int> getRookMoves();
    std::list<int> getBishopMoves();
    std::list<int> getKnightMoves();
    std::list<int> getPawnMoves();
    std::list<int> getKingMoves();
};

class Pawn : public Piece
{
public:
    Pawn(int position, std::string color, char fenName, Board& board);
    ~Pawn();
    std::list<int> getMoves() override;
    void makeMove(int position) override;
    std::list<int> getPotentialMoves() override;
};

class Rook : public Piece
{
public:
    Rook(int position, std::string color, char fenName, Board& board);
    ~Rook();
    std::list<int> getMoves() override;
    void makeMove(int position) override;
    std::list<int> getPotentialMoves() override;
};

class Knight : public Piece
{
public:
    Knight(int position, std::string color, char fenName, Board& board);
    ~Knight();
    std::list<int> getMoves() override;
    void makeMove(int position) override;
    std::list<int> getPotentialMoves() override;
};

class Bishop : public Piece
{
public:
    Bishop(int position, std::string color, char fenName, Board& board);
    ~Bishop();
    std::list<int> getMoves() override;
    void makeMove(int position) override;
    std::list<int> getPotentialMoves() override;
};

class Queen : public Piece
{
public:
    Queen(int position, std::string color, char fenName, Board& board);
    ~Queen();
    std::list<int> getMoves() override;
    void makeMove(int position) override;
    std::list<int> getPotentialMoves() override;
};

class King : public Piece
{
public:
    King(int position, std::string color, char fenName, Board& board);
    ~King();
    std::list<int> getMoves() override;
    void makeMove(int position) override;
    std::list<int> getPotentialMoves() override;
};

#endif // PIECES_H
