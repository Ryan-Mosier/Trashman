#include <iostream>
#include <ncurses.h>

int main() {
    initscr(); // initialize ncurses
    cbreak(); // set keyboard input to immediate mode
    noecho(); // don't echo input characters to the screen

    if(!has_colors()){
        std::cout << "No colors :(\n";
    }else{
        std::cout << "Colors!\n";
    }

    endwin(); // end ncurses mode
    return 0;
}
