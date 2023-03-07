#include <iostream>
#include "Game/Game.h"

int main() {
    std::system("clear");
    Game g;
    g.printMap();
    while(true){
        g.tick();
        g.printMap();
    }
}
