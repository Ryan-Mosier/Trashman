//
// Created by ryanm on 2/16/2023.
//

#ifndef TRASHMAN_MAP_H
#define TRASHMAN_MAP_H

#include <string>

using namespace std;

struct Position{
    int x;
    int y;
};


struct Enemy{
    Position pos;
    string* id;
};

struct TileData{
    bool isVoid;
    bool hasPlayer;
    bool hasEnemy;
    bool hasGarbage;
    bool hasRecycling;

    void setVoid();
    void setEmpty();
    void setPlayer();
};

class Game {
private:
    static const int xsize = 10, ysize = 10;
    TileData Map[xsize][ysize];
    Enemy enemys[4];
    int score;

public:
    Game();
    void printMap();

    void generateLevel();
};

#endif //TRASHMAN_MAP_H
