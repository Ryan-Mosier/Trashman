//
// Created by ryanm on 4/3/2023.
//

#include "Game/Game.h"
#include "Game/colors.h"
#include "Game/debugValues.h"
#include <iostream>

bool debug;
vector<Position> debugPath;

void printPath(){
    std::cout << "\n\n\n";
    for(auto i: debugPath){
        std::cout << "(" << i.x << "," << i.y << ") -> ";
    }
    std::cout << "\n\n\n";
}

int main(){

    debug = true;

    Game g;
    g.printMap();
    while(true){
        g.tick();
        printPath();
        g.printMap();
    }
}