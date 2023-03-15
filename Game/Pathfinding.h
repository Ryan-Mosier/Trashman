//
// Created by ryanm on 3/2/2023.
//



#ifndef TRASHMAN_PATHFINDING_H
#define TRASHMAN_PATHFINDING_H
static const int xsize = 11, ysize = 11;

#include <vector>
#include <cmath>

struct Position;
struct TileData;

struct Node{
    int x,y;

    int f_cost, g_cost, h_cost;
    bool obstacle;
    //f is g+h
    //g is distance from start
    //h is estimated distance from goal

    Node* parent;
};

int distance(Node* node1, Node* node2);

std::vector<Position> a_star(Position startpos, Position goalpos, TileData grid[xsize][ysize]);

void calculateCosts(Node *start,Node *goal,Node map[ysize][xsize]);

#endif //TRASHMAN_PATHFINDING_H
