#include "Game/Game.h"
#include <ncurses.h>
#include "Game/colors.h"

bool debug;
vector<Position> debugPath;

int main() {
    debug = false;

    Game g;
    g.tick();
}
