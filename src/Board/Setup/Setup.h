#ifndef CHESSGAME_SETUP_H
#define CHESSGAME_SETUP_H

#include <map>
#include <string>
#include <list>
#include "../../Pieces/Pieces.h"

class Setup
{
private:
public:
    void addFenToSetup(std::string fen);
    std::map<std::string, std::list<int>> pieces = {};
};

#endif //CHESSGAME_SETUP_H
