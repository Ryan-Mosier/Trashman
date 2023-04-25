//
// Created by ryanm on 3/2/2023.
//

#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <climits>
#include <functional>
#include "Structures.h"
#include <stack>

struct CompareNodes {
    bool operator()(const AStarNode *node1, const AStarNode *node2) const;
};

class NodeHasher {
public:
    size_t operator()(const AStarNode *node) const;
};

struct NodeComparator {
    bool operator()(const AStarNode *lhs, const AStarNode *rhs) const;
};


struct AStarNode {
    int f_cost, g_cost, h_cost;
    bool visited;
    //f is g+h
    //g is distance from start
    //h is estimated distance from goal

    Position position;

    AStarNode *parent;

    std::vector<AStarNode *> adjacent;

    bool operator==(const AStarNode &n1) const {
        return this->position == n1.position;
    }

    explicit AStarNode(Position position) : position(position), f_cost(0), h_cost(0), g_cost(INT_MAX), parent(nullptr), visited(false) {}
};

class Astar {
private:
    std::vector<AStarNode *> nodes;

    std::stack<AStarNode *> modified;

    std::vector<std::vector<int>> vec;

    void addEdge(Position, Position);

    vector<Position> findAdjacent(Position);

    void addNode(Position pos);

    bool BFS(AStarNode *, AStarNode *);

    AStarNode *find(const Position &pos);

    static void resetNode(AStarNode *);

public:

    explicit Astar(std::vector<std::vector<int>>);


    std::vector<Position> aStar(const Position &, const Position &);

    Position random(Position, int);
};

int heuristic(Position startpos, Position goalpos);

int distance(Position a, Position b);

#endif /* ASTAR_H */
