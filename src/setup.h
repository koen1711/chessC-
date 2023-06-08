#ifndef SETUP
#define SETUP

#include "raylib.h"
#include<map>
#include<string>

using namespace std;

class Setup
{
private:
public:
    void addFenToSetup(string fen);
    map<string, string> pieces = {};
};

#endif