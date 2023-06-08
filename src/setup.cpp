#include "setup.h"
#include <cstring>
#include <string>

using namespace std;
void Setup::addFenToSetup(string fen) {
    map<string, string> piecePositions;
    size_t spacePos = fen.find(' ');
    string piecePlacement = fen.substr(0, spacePos);

    // Extract positions from the FEN string
    size_t rank = 7;
    size_t file = 0;
    size_t minFile = 0;
    size_t minRank = 0;

    for (char c : piecePlacement) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (isdigit(c)) {
            file += (c - '0');
        } else {
            string piece;
            piece += c;
            size_t positionX = file % 8 - minFile;
            size_t positionY = rank % 8 - minRank;
            size_t position = positionX + positionY * 8;
            piecePositions[piece] = to_string(position);
            file++;
        }
    }

    pieces = piecePositions;
}


