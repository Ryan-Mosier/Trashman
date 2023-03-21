//
// Created by ryanm on 2/16/2023.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <ncurses.h>
#include <unordered_set>
#include "Game.h"
#include "colors.h"
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include "Pathfinding.h"


using namespace std;

void fullExit() {
    system("clear");
    cout << "EXITING\n";
    endwin(); // end ncurses mode
    exit(42);
}

void TileData::setVoid() {
    isVoid = true;
    entity = nullptr;
    hasGarbage = false;
    hasRecycling = false;
}

void TileData::setEmpty() {
    isVoid = false;
    entity = nullptr;
    hasGarbage = false;
    hasRecycling = false;
}

Game::Game() {
    //TODO: place enemies and player on map
    enemy = new Entity[3];
    player = new Entity;
    tickNum = 0;
    score = 0;
    generateLevel();
    player->prevMove = 'w';
    /*#region entity.id*/
    player->id = "player";
    enemy[0].id = "1";
    enemy[1].id = "2";
    enemy[2].id = "3";
    /*#endregion*/

}

Game::~Game() {
    delete[] enemy;
    delete player;
}

void Game::printMap() {
    //TODO: Render to window

    clear();

    //TODO: colors

    printw("#  #  #  #  #  #  #  #  #  #  #  #  #\n");
    for (auto &i: Map) {
        for (int j = -1; j < xsize + 1; j++) {
            if (j != -1 && j != xsize) {
                if (i[j].isVoid) {
                    printw(" # ");
                } else if (i[j].entity != nullptr) {
                    if (i[j].entity->id == "player") {
                        attron(COLOR_PAIR(YELLOW)); // enable color pair 1 (YELLOW)
                    } else if (i[j].entity->id == "1") {
                        attron(COLOR_PAIR(RED)); // enable color pair 2 (RED)
                    } else if (i[j].entity->id == "2") {
                        attron(COLOR_PAIR(MAGENTA)); // enable color pair 3 (MAGENTA)
                    } else if (i[j].entity->id == "3") {
                        attron(COLOR_PAIR(CYAN)); // enable color pair 4 (CYAN)
                    }
                    printw(" @ ");
                    attroff(COLOR_PAIR(YELLOW)); // disable color pair 1
                    attroff(COLOR_PAIR(RED)); // disable color pair 2
                    attroff(COLOR_PAIR(MAGENTA)); // disable color pair 3
                    attroff(COLOR_PAIR(CYAN)); // disable color pair 4
                } else if (!i[j].isVoid) {
                    printw("   ");
                }
            } else {
                if (j == -1) printw("# ");
                else printw(" #");
            }
        }
        printw("\n");
    }
    printw("#  #  #  #  #  #  #  #  #  #  #  #  #\n");
}

void Game::generateLevel() {

    /*#region Tiles*/

    // -1 = void
    TileData voidTile;
    voidTile.setVoid();
    // 0 = empty
    TileData emptyTile;
    emptyTile.setEmpty();
    // 1 = player
    TileData playerTile;
    playerTile.setEmpty();
    playerTile.entity = player;
    // 7 = enemy1
    TileData enemyTile1;
    enemyTile1.setEmpty();
    enemyTile1.entity = &enemy[0];
    //8 = enemy2
    TileData enemyTile2;
    enemyTile2.setEmpty();
    enemyTile2.entity = &enemy[1];
    //9 = enemy3
    TileData enemyTile3;
    enemyTile3.setEmpty();
    enemyTile3.entity = &enemy[2];
    /*#endregion */

    int arr[xsize][ysize] = {{1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                             {0,  -1, -1, -1, -1, 0,  -1, -1, -1, -1, 0},
                             {0,  0,  -1, 0,  -1, 0,  -1, 0,  -1, 0,  0},
                             {-1, 0,  0,  0,  0,  0,  0,  0,  0,  0,  -1},
                             {-1, -1, 0,  -1, -1, -1, -1, -1, 0,  -1, -1},
                             {0,  0,  7,  -1, 0,  8,  9,  -1, 0,  0,  0},
                             {-1, -1, 0,  -1, -1, -1, -1, -1, 0,  -1, -1},
                             {-1, 0,  0,  0,  0,  0,  0,  0,  0,  0,  -1},
                             {0,  0,  -1, 0,  -1, 0,  -1, 0,  -1, 0,  0},
                             {0,  -1, -1, -1, -1, 0,  -1, -1, -1, -1, 0},
                             {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}};


    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            if (arr[i][j] == -1) Map[i][j] = voidTile;
            else if (arr[i][j] == 0) Map[i][j] = emptyTile;
            else if (arr[i][j] == 1) {
                Map[i][j] = playerTile;
                Position newPos(i, j);
                Map[i][j].entity->pos = newPos;
            } else if (arr[i][j] == 7) {
                Map[i][j] = enemyTile1;
                Position newPos(i, j);
                Map[i][j].entity->pos = newPos;
            } else if (arr[i][j] == 8) {
                Map[i][j] = enemyTile2;
                Position newPos(i, j);
                Map[i][j].entity->pos = newPos;
            } else if (arr[i][j] == 9) {
                Map[i][j] = enemyTile3;
                Position newPos(i, j);
                Map[i][j].entity->pos = newPos;
            }
        }

    }
}

bool validateCharMov(char check) {
    //assuming valid inputs are only wasd
    if (check == 'w') return true;
    if (check == 'a') return true;
    if (check == 's') return true;
    if (check == 'd') return true;
    return false;
}

void Game::moveEntity(Entity *entity, char curr) {
    int xpos = entity->pos.x, ypos = entity->pos.y;


    if (validateCharMov(curr)) {
        entity->prevMove = curr;
    }
    if (!validateCharMov(entity->prevMove)) {
        return;
    }

    int newx = xpos, newy = ypos;
    if (entity->prevMove == 'w') {
        newx--;
    } else if (entity->prevMove == 's') {
        newx++;
    } else if (entity->prevMove == 'a') {
        newy--;
    } else if (entity->prevMove == 'd') {
        newy++;
    }

    if ((entity->prevMove == 'a') && (xpos == 5) && (ypos == 0)) {
        newx = 5;
        newy = 10;
    } else if ((entity->prevMove == 'd') && (xpos == 5) && (ypos == 10)) {
        newx = 5;
        newy = 0;
    }

    if (newy > 10) {
        newy = 10;
    } else if (newy < 0) {
        newy = 0;
    } else if (newx > 10) {
        newx = 10;
    } else if (newx < 0) {
        newx = 0;
    }

    if (Map[newx][newy].isVoid) {
        return;
    }
    Map[xpos][ypos].entity = nullptr;
    if (entity->id != "player") {
        if (Map[newx][newy].entity->id == "player") {
            kill();
        }
    }
    Map[newx][newy].entity = entity;
    Position newPos(newx, newy);
    entity->pos = newPos;

    if (entity->id == "player") {
        if (Map[newx][newy].hasGarbage) {
            score += 10;
            Map[newx][newy].hasGarbage = false;
        }

    }

    //TODO: collisionlogic?
}

void Game::moveEntity(Entity *entity, Position newpos) {
    int difference = abs(entity->pos.x - newpos.x) + abs(entity->pos.y - newpos.y);
    if (difference < 2) {
        Map[entity->pos.x][entity->pos.y].entity = nullptr;
        entity->pos = newpos;
        if (Map[newpos.x][newpos.y].entity == player) {
            kill();
        }
        Map[entity->pos.x][entity->pos.y].entity = entity;
    } else {
        fullExit();
    }
}

bool isValidInput(char input) {
    std::unordered_set<char> validInputs = {'w', 'a', 's', 'd', 'k'};
    return (validInputs.find(input) != validInputs.end());
}

std::vector<char> getPressedKeys() {
    std::vector<char> keys;
    char ch;

    nodelay(stdscr, true); // don't restart the timeout everytime a key is pressed

    timeout(10);

    clock_t start = clock();

    auto start_time = std::chrono::steady_clock::now();

    while (true) {
        ch = getch();
        if (ch != ERR) {
            keys.push_back(ch);
        }

        // measure elapsed time
        clock_t end = clock();
        double elapsedSeconds = double(end - start) / CLOCKS_PER_SEC;
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() >=
            1) {
            break;
        }

    }
    return keys;
}

char findLatestValidKey(std::vector<char> vec) {
    while (!vec.empty()) {
        if (isValidInput(vec.back())) {
            return vec.back();
        } else {
            vec.pop_back();
        }
    }
    return '\0';
}


void Game::tick() {
    char in = '\0';
    if (tickNum == 0) {
        in = findLatestValidKey(getPressedKeys());
        moveEntity(player, in);
    } else if (tickNum % 2 == 0) {
        in = findLatestValidKey(getPressedKeys());
    }

    if (tickNum % 2 == 0) {
        moveEntity(player, in);
    }

    if (tickNum % 3 == 0) {
        Position outOfBox(5, 2);
        if (tickNum == 3) {
            Map[enemy[0].pos.x][enemy[0].pos.y].entity = nullptr;
            enemy[0].pos = outOfBox;
            Map[outOfBox.x][outOfBox.y].entity = &enemy[0];
        } else if (tickNum == 12) {
            Map[enemy[1].pos.x][enemy[1].pos.y].entity = nullptr;
            enemy[1].pos = outOfBox;
            Map[outOfBox.x][outOfBox.y].entity = &enemy[1];
        } else if (tickNum == ) {
            Map[enemy[2].pos.x][enemy[2].pos.y].entity = nullptr;
            enemy[2].pos = outOfBox;
            Map[outOfBox.x][outOfBox.y].entity = &enemy[2];
        }
        for (int i = 0; i < 3; i++) {
            vector<Position> path;
            path = a_star(player->pos, enemy[i].pos, Map);
            //vector is sorted start --> end
            if (path.size() > 2) {
                moveEntity(&enemy[i], path[1]);
            }
            //TODO: verify that pathfinding works
        }
    }


    tickNum++;


    //TODO:finish tick function (what do I mean by this)
}

void Game::movePlayer(char input) {
    moveEntity(player, input);
}


void Game::kill() {
    system("clear");
    cout << "You Died!\n========\nYour Score: " << score << "\n";
    this_thread::sleep_for(chrono::seconds(2));
    //TODO: print endscreen
    endwin(); // end ncurses mode
    exit(1);
}
