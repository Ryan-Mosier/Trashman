//
// Created by ryanm on 3/2/2023.
//

#include "Pathfinding.h"
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Game.h"

using namespace std;

int distance(Node *node1, Node *node2) {
    return abs(node1->x - node2->x) + abs(node1->y - node2->y);
}

vector<vector<Node>>
convertMap(Position startpos, Position goalpos, TileData grid[xsize][ysize], Node *&start, Node *&goal) {

    vector<vector<Node>> map(xsize, vector<Node>(ysize));
    goal = &map[goalpos.x][goalpos.y];
    start = &map[startpos.x][startpos.y];

    //set obstacle
    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            if (grid[i][j].isVoid) {
                map[i][j].obstacle = true;
            } else {
                map[i][j].obstacle = false;
            }
        }
    }

    //set location
    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            map[i][j].x = i;
            map[i][j].y = j;
        }
    }

    //find costs
    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            map[i][j].g_cost = distance(start, &map[i][j]);
            map[i][j].h_cost = distance(&map[i][j], goal);
            map[i][j].f_cost = map[i][j].g_cost + map[i][j].h_cost;
        }
    }
    return map;
}

void calculateCosts(Node *start, Node *goal, Node map[xsize][ysize]) {
    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            map[i][j].g_cost = distance(start, &map[i][j]);
            map[i][j].h_cost = distance(&map[i][j], goal);
            map[i][j].f_cost = map[i][j].g_cost + map[i][j].h_cost;
        }
    }
}

//g is distance from start
//h is estimated distance from goal

std::vector<Node *> a_star(Position startpos, Position goalpos, TileData grid[xsize][ysize]) {
    Node *start_node;
    Node *goal_node;
    vector<vector<Node>> map;
    map = convertMap(startpos, goalpos, grid, start_node, goal_node);

    vector<Node *> path;
    priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<pair<int, Node *>>> pq;
    start_node->g_cost = 0;
    start_node->h_cost = distance(start_node, goal_node);
    start_node->f_cost = start_node->g_cost + start_node->h_cost;
    pq.push({start_node->f_cost, start_node});
    while (!pq.empty()) {
        Node *current_node = pq.top().second;
        pq.pop();
        if (current_node == goal_node) {
            // Reconstruct the path
            while (current_node != start_node) {
                path.push_back(current_node);
                current_node = current_node->parent;
            }
            path.push_back(start_node);
            reverse(path.begin(), path.end());
            break;
        }
        current_node->obstacle = true; // Mark current node as obstacle to avoid revisiting it
        int x = current_node->x;
        int y = current_node->y;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;
                if (x + i < 0 || x + i >= map.size() || y + j < 0 || y + j >= map[0].size()) continue;
                Node *neighbor = &map[x + i][y + j];
                if (neighbor->obstacle) continue;
                int tentative_g_cost = current_node->g_cost + distance(current_node, neighbor);
                if (tentative_g_cost < neighbor->g_cost) {
                    neighbor->parent = current_node;
                    neighbor->g_cost = tentative_g_cost;
                    neighbor->h_cost = distance(neighbor, goal_node);
                    neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
                    pq.push({neighbor->f_cost, neighbor});
                }
            }
        }
    }
    return path;
}