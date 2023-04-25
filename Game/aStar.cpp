#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <functional>
#include <limits.h>
#include <stack>
#include "Structures.h"
#include "aStar.h"

using namespace std;

bool CompareNodes::operator()(const AStarNode *node1, const AStarNode *node2) const {
    return node1->f_cost > node2->f_cost;
}

size_t NodeHasher::operator()(const AStarNode *node) const {
    std::size_t h1 = std::hash<int>{}(node->position.x);
    std::size_t h2 = std::hash<int>{}(node->position.y);
    return h1 ^ (h2 << 1);
}

bool NodeComparator::operator()(const AStarNode *lhs, const AStarNode *rhs) const {
    return lhs->position == rhs->position;
}


int heuristic(Position startpos, Position goalpos) {
    int dx = abs(startpos.x - goalpos.x);
    int dy = abs(startpos.y - goalpos.y);
    return dx + dy;
}

int distance(Position a, Position b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool checkIfEdgeExists(AStarNode *r, int x2, int y2) {
    for (auto i: r->adjacent) {
        if (i->position.x == x2) {
            if (i->position.y == y2) return true;
        }
    }
    return false;
}

vector<Position> Astar::findAdjacent(Position p1) {
    int x = p1.x, y = p1.y;
    vector<Position> neighbors;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (abs(i) == abs(j))continue;
            if (x + i < 0 || x + i >= xsize || y + j < 0 || y + j >= ysize) continue;
            if (vec[y + j][x + i] != 2) neighbors.emplace_back(x + i, y + j);
        }
    }
    return neighbors;
}

void Astar::addEdge(Position p1, Position p2) {
    int x1 = p1.x, y1 = p1.y;
    int x2 = p2.x, y2 = p2.y;
    for (unsigned int i = 0; i < nodes.size(); i++) {
        if (nodes[i]->position.x == x1 && nodes[i]->position.y == y1) {
            for (unsigned int j = 0; j < nodes.size(); j++) {
                if (i != j && nodes[j]->position.x == x2 && nodes[j]->position.y == y2) {
                    if (!checkIfEdgeExists(nodes[i], x2, y2)) {
                        AStarNode *av;
                        av = nodes[j];
                        nodes[i]->adjacent.push_back(av);
                        // another vertex for edge in other direction
                        AStarNode *av2;
                        av2 = nodes[i];
                        nodes[j]->adjacent.push_back(av2);
                        break;
                    }
                }
            }
        }
    }
}

void Astar::addNode(Position pos) {
    auto *node = new AStarNode(pos);
    nodes.emplace_back(node);
}

Astar::Astar(std::vector<std::vector<int>> vec) : vec(vec) {
    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec[i].size(); j++) {
            if (this->vec[i][j] == 2) continue;
            addNode({j, i});
        }
    }
    for (auto i: nodes) {
        vector<Position> neighbors = findAdjacent(i->position);
        for (auto j: neighbors) {
            addEdge(i->position, j);
        }
        if (i->adjacent.empty()) {
            nodes.end() = std::remove(nodes.begin(), nodes.end(), i);
        }
    }
}

void Astar::resetNode(AStarNode *node) {
    auto temp = node->adjacent;
    *node = AStarNode(node->position);
    node->adjacent = temp;
}

AStarNode *Astar::find(const Position &pos) {
    for (auto i: nodes) {
        if (i->position == pos) return i;
    }
    return nullptr;
}

bool Astar::BFS(AStarNode *n1, AStarNode *n2) {
    if (!n1 || !n2) {
        cerr << "Invalid inputs\n";
        return false;
    }
    for (auto i: nodes) {
        i->visited = false;
    }

    queue<AStarNode *> q;
    modified.emplace(n1);
    n1->visited = true;
    q.push(n1);
    while (!q.empty()) {
        AStarNode *curr = q.front();
        q.pop();
        if (curr == n2) return true;
        for (auto i: curr->adjacent) {
            if (i->visited) continue;
            modified.emplace(i);
            i->parent = curr;
            i->visited = true;
            q.push(i);
        }
    }
    return false;
}

std::vector<Position> Astar::aStar(const Position &start, const Position &goal) {
    AStarNode *startNode = find(start);
    AStarNode *goalNode = find(goal);
    if (!startNode || !goalNode) {
        return {};
    }
    if (!BFS(startNode, goalNode)) {
        cerr << "Bad Graph!!";
        return {};
    } else {
        std::vector<Position> path;
        AStarNode *node = goalNode;
        while (node != nullptr) {
            path.push_back(node->position);
            node = node->parent;
        }
        std::reverse(path.begin(), path.end());
        while (!modified.empty()) {
            resetNode(modified.top());
            modified.pop();
        }
        return path;
    }


    std::priority_queue<AStarNode *, std::vector<AStarNode *>, CompareNodes> openSet;
    std::unordered_set<AStarNode *, NodeHasher, NodeComparator> closedSet;

    modified.emplace(startNode);
    startNode->g_cost = 0;
    startNode->h_cost = heuristic(start, goal);
    startNode->f_cost = startNode->g_cost + startNode->h_cost;
    openSet.push(startNode);

    while (!openSet.empty()) {
        AStarNode *currentNode = openSet.top();
        openSet.pop();

        if (currentNode == goalNode) {
            // Build path from start to goal
            std::vector<Position> path;
            AStarNode *node = currentNode;
            while (node != nullptr) {
                path.push_back(node->position);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());
            while (!modified.empty()) {
                resetNode(modified.top());
                modified.pop();
            }
            return path;
        }

        closedSet.insert(currentNode);

        for (AStarNode *neighborNode: currentNode->adjacent) {
            if (closedSet.find(neighborNode) != closedSet.end()) {
                // Neighbor has already been evaluated
                continue;
            }

            int tentativeGCost = currentNode->g_cost + distance(currentNode->position, neighborNode->position);
            int neighborHCost = heuristic(neighborNode->position, goal);

            if (tentativeGCost < neighborNode->g_cost) {
                // set does not contain the neighbor or the new path is better
                modified.emplace(neighborNode);
                neighborNode->g_cost = tentativeGCost;
                neighborNode->h_cost = neighborHCost;
                neighborNode->f_cost = neighborNode->g_cost + neighborNode->h_cost;
                neighborNode->parent = currentNode;

                if (openSet.empty() || openSet.top()->f_cost > neighborNode->f_cost) {
                    openSet.push(neighborNode);
                }
            }
        }
    }


    while (!modified.empty()) {
        resetNode(modified.top());
        modified.pop();
    }
    return {};
}

Position Astar::random(Position pos, int in) {
    AStarNode *node = find(pos);
    in %= node->adjacent.size();
    return node->adjacent[in]->position;
}
