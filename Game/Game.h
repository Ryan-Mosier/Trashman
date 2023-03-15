//
// Created by ryanm on 2/16/2023.
//

#ifndef TRASHMAN_GAME_H
#define TRASHMAN_GAME_H

#include <string>
#include "Pathfinding.h"

using namespace std;

struct Position {
    int x;
    int y;
    Position(int x_, int y_) {
        x = x_;
        y = y_;
    }
    Position() = default;
};

void fullExit();

struct Entity {
    Position pos;
    string id;
    int deathlength;
    char prevMove;
    // player == player
    //ghost 1 == 1
    //ghost 2 == 2
    //ghost 3 == 3
};

struct TileData {
    bool isVoid;

    Entity *entity = nullptr;


    bool hasGarbage;

    bool hasRecycling;

    void setVoid();

    void setEmpty();

    void setPlayer();

    // garbage is the pellets
    // recycling is the cherry
};

class Game {
private:

    //TODO:lives?
    int tickNum;
    TileData Map[xsize][ysize];
    Entity *enemy;
    // array of size 3
    Entity *player;

    /*
     * Pac-Dot = 10 Pts
     * Power Pellet = 50 Pts
     * 1st Ghost = 200 Pts
     * 2nd Ghost = 400 Pts
     * 3rd Ghost = 800 Pts
     * 4th Ghost = 1600 Pts
     */
    int score;

public:

    Game();

    ~Game();

    void printMap();

    void moveEntity(Entity *entity, char curr);
    void moveEntity(Entity *entity, Position);

    void generateLevel();

    void movePlayer(char input);

    void tick();

    void kill();
};

#endif //TRASHMAN_GAME_H
