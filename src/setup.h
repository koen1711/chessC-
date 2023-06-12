#ifndef SETUP
#define SETUP

#include "raylib.h"
#include <map>
#include <string>
#include <list>

class Setup
{
private:
public:
    void addFenToSetup(std::string fen);
    std::map<std::string, std::list<int>> pieces = {};
};

#endif