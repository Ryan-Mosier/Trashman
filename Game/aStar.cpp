//
// Created by ryanm on 3/2/2023.
//

#include "aStar.h"
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Game.h"
#include <iostream>

using namespace std;

int calcDistance(AStarNode *node1, AStarNode *node2) {
    return abs(node1->position.x - node2->position.x) + abs(node1->position.y - node2->position.y);
}

int calcDistance(Position *node1, Position *node2) {
    return abs(node1->x - node2->x) + abs(node1->y - node2->y);
}


void convertMap(Position startpos, Position goalpos, TileData map[xsize][ysize], TileData *&start, TileData *&goal) {

    goal = &map[goalpos.x][goalpos.y];
    start = &map[startpos.x][startpos.y];

    //set empty
    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            AStarNode &Node = map[i][j].aStarNode;
            Node.f_cost = 0;
            Node.g_cost = 1000000;
            Node.h_cost = 0;
            Node.parent = nullptr;
            Node.obstacle = map[i][j].isVoid;
        }
    }

//    for (int i = 0; i < xsize; i++) {
//        std::cout << "# ";
//        for (int j = 0; j < ysize; j++) {
//            if(map[i][j].aStarNode.obstacle){
//                std::cout << " # ";
//            }else{
//                std::cout << "   ";
//            }
//        }
//        std::cout << " #";
//        std::cout << "\n";
//    }
}

//g is distance from start
//h is estimated distance from goal

std::vector<Position> convertPath(std::vector<AStarNode *> in) {
    std::vector<Position> out;
    while (!in.empty()) {
        Position temp(in.back()->position.x, in.back()->position.y);
        out.push_back(temp);
        in.pop_back();
    }
    return out;
}

std::vector<Position> a_star(Position startpos, Position goalpos, TileData map[xsize][ysize]) {
    TileData *start;
    TileData *goal;
    convertMap(startpos, goalpos, map, start, goal);
    AStarNode &start_node = start->aStarNode;
    AStarNode &goal_node = goal->aStarNode;
    vector<AStarNode *> path;
    priority_queue<pair<int, AStarNode *>, vector<pair<int, AStarNode *>>, greater<pair<int, AStarNode *>>> pq;

    start_node.g_cost = 0;
    start_node.h_cost = calcDistance(&start->aStarNode, &goal->aStarNode);
    start_node.f_cost = start_node.g_cost + start_node.h_cost;
    pq.push({start_node.f_cost, &start_node});
    while (!pq.empty()) {
        AStarNode *current_node = pq.top().second;
        pq.pop();
        if (current_node == &goal_node) {
            // Reconstruct the path
            while (current_node != &start_node) {
                path.push_back(current_node);
                current_node = current_node->parent;
            }
            path.push_back(&start_node);
            break;
        }
        current_node->obstacle = true; // Mark current node as obstacle to avoid revisiting it
        int x = current_node->position.x;
        int y = current_node->position.y;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (abs(i) == abs(j)) continue;
                if (i == 0 && j == 0) continue;
                if (x + i < 0 || x + i >= xsize || y + j < 0 || y + j >= ysize) continue;
                if (map[x + i][y + j].aStarNode.obstacle) continue;
                AStarNode *neighbor = &map[x + i][y + j].aStarNode;
                int tentative_g_cost = current_node->g_cost + calcDistance(current_node, neighbor);
                if (tentative_g_cost < neighbor->g_cost) {
                    neighbor->parent = current_node;
                    neighbor->g_cost = tentative_g_cost;
                    neighbor->h_cost = calcDistance(neighbor, &goal_node);
                    neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
                    pq.push({neighbor->f_cost, neighbor});
                }
            }
        }
    }
    vector<Position> posData = convertPath(path);
    return posData;
}
