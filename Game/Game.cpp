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
#include "aStar.h"
#include "debugValues.h"


using namespace std;

void fullExit() {
    system("clear");
    cout << "EXITING\n";
    endwin(); // end ncurses mode
    exit(42);
}


Game::Game() {
    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            Map[i][j] = TileData(i, j);
        }
    }
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

void Game::DebugprintMap() {
    system("clear");
    cout << "#  #  #  #  #  #  #  #  #  #  #  #  #\n";
    for (auto &i: Map) {
        for (int j = -1; j < xsize + 1; j++) {
            if (j != -1 && j != xsize) {
                if (i[j].isVoid) {
                    cout << " # ";
                } else if (i[j].entity != nullptr) {
                    if (i[j].entity->id == "player") {
                        cout << "\033[33m"; //YELLOW
                    } else if (i[j].entity->id == "1") {
                        cout << "\033[31m"; //RED
                    } else if (i[j].entity->id == "2") {
                        cout << "\033[35m"; //MAGENTA
                    } else if (i[j].entity->id == "3") {
                        cout << "\033[36m"; //CYAN
                    }
                    cout << " @ ";
                    cout << "\033[0m";
                } else if (!i[j].isVoid) {
                    if (!i[j].hasGarbage) {
                        cout << "   ";
                    } else {
                        cout << " * ";
                    }

                }
            } else {
                if (j == -1) cout << "# ";
                else cout << " #";
            }
        }
        cout << "\n";
    }
    cout << "#  #  #  #  #  #  #  #  #  #  #  #  #\n";
}

void Game::printMap() {

    if (debug) {
        DebugprintMap();
        return;
    }

    //TODO: Render to window
    //remember endscreen in kill()
    //remember some sort of intro screen before game launch

    clear();

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
                    if (!i[j].hasGarbage) {
                        printw("   ");
                    } else {
                        attron(COLOR_PAIR(YELLOW));
                        printw(" * ");
                        attroff(COLOR_PAIR(YELLOW));
                    }

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


    /*#endregion */

    int arr[xsize][ysize] = {{1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                             {0,  -1, -1, -1, -1, 0,  -1, -1, -1, -1, 0},
                             {0,  0,  -1, 0,  -1, 0,  -1, 0,  -1, 0,  0},
                             {-1, 0,  0,  0,  0,  0,  0,  0,  0,  0,  -1},
                             {-1, -1, 0,  -1, -1, -1, -1, -1, 0,  -1, -1},
                             {0,  0,  0,  -1, 7,  8,  9,  -1, 0,  0,  0},
                             {-1, -1, 0,  -1, -1, -1, -1, -1, 0,  -1, -1},
                             {-1, 0,  0,  0,  0,  0,  0,  0,  0,  0,  -1},
                             {0,  0,  -1, 0,  -1, 0,  -1, 0,  -1, 0,  0},
                             {0,  -1, -1, -1, -1, 0,  -1, -1, -1, -1, 0},
                             {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}};


    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            if (arr[i][j] == -1) Map[i][j].setVoid();
            else if (arr[i][j] == 0) {
                Map[i][j].setEmpty();
                remainingGarbage++;
            } else if (arr[i][j] == 1) {
                Map[i][j].setEmpty();
                Map[i][j].entity = player;
                Map[i][j].entity->pos = Map[i][j].pos;

            } else if (arr[i][j] == 7) {
                Map[i][j].setEmpty();
                Map[i][j].entity = &enemy[0];
                Map[i][j].entity->pos = Map[i][j].pos;
            } else if (arr[i][j] == 8) {
                Map[i][j].setEmpty();
                Map[i][j].entity = &enemy[1];
                Map[i][j].entity->pos = Map[i][j].pos;
            } else if (arr[i][j] == 9) {
                Map[i][j].setEmpty();
                Map[i][j].entity = &enemy[2];
                Map[i][j].entity->pos = Map[i][j].pos;
            }
            //place garbage
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
    //check collision

    Map[xpos][ypos].entity = nullptr;
    Map[newx][newy].entity = entity;
    Position newPos(newx, newy);
    entity->pos = newPos;

    if (entity->id == "player") {
        if (Map[newx][newy].hasGarbage) {
            score += 10;
            Map[newx][newy].hasGarbage = false;
            remainingGarbage--;
            if (remainingGarbage == 0) {
                tickNum = 0;
                generateLevel();
            }
        }
    }
}

void Game::moveEntity(Entity *entity, Position newpos) {
    int distance = abs(entity->pos.x - newpos.x) + abs(entity->pos.y - newpos.y);
    if (distance > 1) {
        return;
    }
    if (Map[newpos.x][newpos.y].entity != nullptr) {
        if (Map[newpos.x][newpos.y].entity == player) {
            kill();
        }
        return;
    }
    Map[entity->pos.x][entity->pos.y].entity = nullptr;
    entity->pos = newpos;
    Map[entity->pos.x][entity->pos.y].entity = entity;
}

Position Game::predictPosition(Entity *entity, char prev) {
    int xpos = entity->pos.x, ypos = entity->pos.y;
    for (int i = 0; i < 4; i++) {
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
            break;
        }
        xpos = newx;
        ypos = newy;
    }
    return {xpos, ypos};
}

bool isValidInput(char input) {
    std::unordered_set<char> validInputs = {'w', 'a', 's', 'd', 'k'};
    return (validInputs.find(input) != validInputs.end());
}

std::vector<char> getPressedKeys() {
    std::vector<char> keys;
    char ch;
    if (debug) {
        char ch = '\0';
        while (!isValidInput(ch)) {
            cin.clear();
            cin >> ch;
        }
        keys.push_back(ch);
        return keys;
    }


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
        moveEntity(player, in);
    }

    if (tickNum % 3 == 0) {
        Position outOfBox(5, 2);
        if (tickNum == 3) {

            Map[enemy[0].pos.x][enemy[0].pos.y].entity = nullptr;
            enemy[0].pos = outOfBox;
            Map[outOfBox.x][outOfBox.y].entity = &enemy[0];
            enemy[0].deathlength = 0;
            enemy[0].dead = false;
        } else if (tickNum == 12) {
            //TODO: work out edge cases and copy this
            if (Map[outOfBox.x][outOfBox.y].entity == nullptr) {
                Map[enemy[1].pos.x][enemy[1].pos.y].entity = nullptr;
                enemy[1].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[1];
            }
        } else if (tickNum == 21) {
            Map[enemy[2].pos.x][enemy[2].pos.y].entity = nullptr;
            enemy[2].pos = outOfBox;
            Map[outOfBox.x][outOfBox.y].entity = &enemy[2];
        }

        //move enemies
        if (!enemy[0].dead) {
            vector<Position> path;

            path = a_star(enemy[0].pos, player->pos, Map);
            if (debug) {
                debugPath = path;
            }
            //vector is sorted start --> end
            if (!path.empty()) {
                moveEntity(&enemy[0], path[1]);
            }
        } else {
            enemy[0].deathlength++;
            if (enemy[0].deathlength > 10) {
                Position respawn(0, 0);
                Map[enemy[0].pos.x][enemy[0].pos.y].entity = nullptr;
                enemy[0].pos = respawn;
                Map[respawn.x][respawn.y].entity = &enemy[0];
                enemy[0].deathlength = 0;
                enemy[0].dead = false;
            }
        }
        if (!enemy[1].dead) {
            vector<Position> path;
            path = a_star(enemy[1].pos, predictPosition(player, player->prevMove), Map);
            if (!path.empty()) {
                moveEntity(&enemy[0], path[1]);
            }

        } else {
            enemy[1].deathlength++;
            if (enemy[1].deathlength > 10) {
                Position respawn(0, 10);
                Map[enemy[1].pos.x][enemy[1].pos.y].entity = nullptr;
                enemy[1].pos = respawn;
                Map[respawn.x][respawn.y].entity = &enemy[1];
                enemy[1].deathlength = 0;
                enemy[1].dead = false;
            }
        }
        if (!enemy[2].dead) {
            //TODO: pathfinding for 3
            int distance = calcDistance(&enemy[2].pos, &player->pos);
            if (distance < 4) {
                vector<Position> path;

                path = a_star(enemy[2].pos, player->pos, Map);
                //vector is sorted start --> end
                if (!path.empty()) {
                    moveEntity(&enemy[2], path[1]);
                }
            } else {
                vector<Position> possible;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (abs(i) == abs(j)) continue;
                        if (i == 0 && j == 0) continue;
                        if (enemy->pos.x + i < 0 || enemy->pos.x + i >= xsize || enemy->pos.y + j < 0 || enemy->pos.y + j >= ysize) continue;
                        possible.emplace_back(enemy[2].pos.x + i, enemy[2].pos.y + j);
                    }
                }
                int random = rand() % int(possible.size());
                moveEntity(&enemy[2], possible[random]);
            }
        } else {
            enemy[2].deathlength++;
            if (enemy[2].deathlength > 10) {
                Position respawn(10, 0);
                Map[enemy[2].pos.x][enemy[0].pos.y].entity = nullptr;
                enemy[2].pos = respawn;
                Map[respawn.x][respawn.y].entity = &enemy[2];
                enemy[2].deathlength = 0;
                enemy[2].dead = false;
            }
        }
    }
    tickNum++;
}

void Game::movePlayer(char input) {
    moveEntity(player, input);
}


void Game::kill() {
    system("clear");
    cout << "You Died!\n========\nYour Score: " << score << "\n";
    this_thread::sleep_for(chrono::seconds(2));
    //TODO: add endscreen (render)
    endwin(); // end ncurses mode
    exit(1);
}
