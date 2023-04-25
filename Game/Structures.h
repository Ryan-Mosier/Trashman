//
// Created by ryanm on 3/31/2023.
//



#ifndef MYPROJECT_STRUCTURES_H
#define MYPROJECT_STRUCTURES_H

#include <string>
#include <vector>

struct AStarNode;
struct Position;
struct Entity;
struct TileData;

using namespace std;

struct Position {
    int x;
    int y;

    Position(int x_, int y_) {
        x = x_;
        y = y_;
    }

    Position() = default;

    bool operator==(const Position &other) const {
        return (x == other.x && y == other.y);
    }

    bool operator<=>(Position other) const {
        // Check if other is above, below, left, or right of this position
        if (other.x == x && other.y == y - 1) return true; // above
        if (other.x == x && other.y == y + 1) return true; // below
        if (other.x == x - 1 && other.y == y) return true; // left
        if (other.x == x + 1 && other.y == y) return true; // right
        return false;
    }

    [[nodiscard]] Position reverse() const {
        return {y,x};
    }
};

struct Entity {
    Position pos;
    string id;
    int deathlength;
    bool dead;
    char prevMove;
    // player == player
    //ghost 1 == 1
    //ghost 2 == 2
    //ghost 3 == 3
};

struct TileData {
    Position pos;

    Entity *entity;

    bool isVoid;
    bool hasGarbage;
    bool hasPP;


    TileData() = default;

    TileData(int x, int y) : pos(x, y), entity(nullptr) {}

    void setVoid();

    void setEmpty();

    // garbage is the pellets
    // recycling is the cherry
};

//Global Variables
static const int xsize = 32, ysize = 18;
static const Position outOfBox(3, 10);
static const Position topLeft(1, 1), topRight(1, ysize - 2);
static const Position bottomLeft(xsize - 2, 1), bottomRight(xsize - 2, ysize - 2);
static const Position topLeftPass(4, 0), topRightPass(4, xsize - 1);
static const Position bottomLeftPass(13, 0), bottomRightPass(13, xsize - 1);
static const Position enemyBox[3] = {{9, 6},
                                     {9, 30},
                                     {8, 30}};

//AStarNode::AStarNode() {
//    position = Position(0,0);
//}

#endif //MYPROJECT_STRUCTURES_H
