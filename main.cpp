#include <iostream>
#include "Game/Game.h"
#include <ncurses.h>
#include "Game/colors.h"

int main() {
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
