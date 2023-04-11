//
// Created by ryanm on 3/2/2023.
//



#ifndef TRASHMAN_PATHFINDING_H
#define TRASHMAN_PATHFINDING_H

#include <vector>
#include <cmath>
#include "Game.h"
#include "Structures.h"


int calcDistance(AStarNode *node1, AStarNode *node2);
int calcDistance(Position *node1, Position *node2);

std::vector<Position> a_star(Position startpos, Position goalpos, TileData grid[xsize][ysize]);

#endif //TRASHMAN_PATHFINDING_H
