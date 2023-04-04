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

struct AStarNode {
    AStarNode();

    int f_cost, g_cost, h_cost;
    bool obstacle;
    //f is g+h
    //g is distance from start
    //h is estimated distance from goal

    Position position;

    AStarNode *parent;

    AStarNode(TileData &tileData);

};

struct TileData {
    Position pos;

    Entity *entity;

    bool isVoid;
    bool hasGarbage;
    bool hasRecycling;

    AStarNode aStarNode = AStarNode();

    TileData() {

    }

    TileData(int x, int y) {
        pos = Position(x, y);
        entity = nullptr;
        aStarNode = AStarNode(*this);
    }

    void setVoid();

    void setEmpty();

    void setPlayer();

    // garbage is the pellets
    // recycling is the cherry
};

//AStarNode::AStarNode() {
//    position = Position(0,0);
//}

#endif //MYPROJECT_STRUCTURES_H
