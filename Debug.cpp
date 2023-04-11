//
// Created by ryanm on 4/3/2023.
//

#include "Game/Game.h"
#include "Game/colors.h"
#include "Game/debugValues.h"
#include <iostream>

bool debug;
vector<vector<Position>> debugPath(3);
vector<Position> entityPos(4);

void printPath(vector<Position> path) {
    for (auto i: path) {
        std::cout << "(" << i.x << "," << i.y << ") -> ";
    }
}

void printDebugInfo() {
    std::cout << "\n";
    std::cout << "Player: " << "(" << entityPos[0].x << "," << entityPos[0].y << ")\n";
    std::cout << "Enemy 1: " << "(" << entityPos[1].x << "," << entityPos[1].y << ") --- ";
    printPath(debugPath[0]);
    std::cout << "\nEnemy 2: " << "(" << entityPos[2].x << "," << entityPos[2].y << ") --- ";
    printPath(debugPath[1]);
    std::cout << "\nEnemy 3: " << "(" << entityPos[3].x << "," << entityPos[3].y << ") --- ";
    printPath(debugPath[2]);
    std::cout << "\n\n\n";
}

int main() {

    debug = true;

    Game g;
    g.printMap();
    printDebugInfo();
    while (true) {
        g.tick();
        g.printMap();
        printDebugInfo();
    }
}