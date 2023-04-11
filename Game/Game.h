//
// Created by ryanm on 2/16/2023.
//

#ifndef TRASHMAN_GAME_H
#define TRASHMAN_GAME_H

#include <string>
#include "aStar.h"
#include "Structures.h"

using namespace std;

void fullExit();

class Game {
private:


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
     */

    int score;
    int remainingGarbage;
    bool powerPellet;
    int remainingPPDuration;

public:

    Game();

    ~Game();

    void DebugprintMap();

    void printMap();

    void moveEntity(Entity *entity, char curr);

    Position predictPosition(Entity *entity, char prev);

    void moveEntity(Entity *entity, Position);

    void generateLevel();

    void killEnemy(int i);

    void tick();

    void kill();
};

#endif //TRASHMAN_GAME_H
