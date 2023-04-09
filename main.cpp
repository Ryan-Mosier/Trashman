#include <iostream>
#include "Game/Game.h"
#include <ncurses.h>
#include "Game/colors.h"
#include "Game/debugValues.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

bool debug;
vector<vector<Position>> debugPath(3);
vector<Position> entityPos(4);

int main() {
    debug = false;
    initscr(); // initialize ncurses
    cbreak(); // set keyboard input to immediate mode
    noecho(); // don't echo input characters to the screen
    START_COLORS

    Game g;
    g.printMap();
    while(true){
        g.tick();
        g.printMap();
    }
}

#pragma clang diagnostic pop