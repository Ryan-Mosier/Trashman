//
// Created by ryanm on 2/16/2023.
//

#ifndef TRASHMAN_MAP_H
#define TRASHMAN_MAP_H

#include <string>


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

    // garbage is the pellets
    bool hasGarbage;
    // recycling is the cherry
    bool hasRecycling;

    void setVoid();

    void setEmpty();

    void setPlayer();
};

class Game {
private:

    //TODO:lives?
    int tickNum;
    static const int xsize = 11, ysize = 11;
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

    void generateLevel();

    void movePlayer(char input);

    void tick();

    void kill();
};

#endif //TRASHMAN_MAP_H
