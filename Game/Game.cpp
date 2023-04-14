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
    for (int i = 0; i < 3; i++) {
        enemy[i].dead = true;
        enemy[i].deathlength = -1000;
    }
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
    powerPellet = false;
    remainingPPDuration = 0;
}

Game::~Game() {
    delete[] enemy;
    delete player;
}

void Game::DebugprintMap() {
    system("clear");
    cout << "#  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #\n";
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
                        cout << "\033[33m";
                        cout << " * ";
                        cout << "\033[0m";
                    }

                }
            } else {
                if (j == -1) cout << "# ";
                else cout << " #";
            }
        }
        cout << "\n";
    }
    cout << "#  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #\n";
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

    printw("#  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #\n");
    for (auto &i: Map) {
        for (int j = -1; j < xsize + 1; j++) {
            if (j != -1 && j != xsize) {
                if (i[j].isVoid) {
                    printw(" # ");
                } else if (i[j].hasPP) {
                    attron(COLOR_PAIR(GREEN));
                    printw(" $ ");
                    attroff(COLOR_PAIR(GREEN));
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
    printw("#  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #\n");
}

void Game::generateLevel() {

    /*#region Tiles*/

    // 3 = PP
    TileData pp;
    pp.setEmpty();
    pp.hasPP = true;
    pp.hasGarbage = false;

    // 2 = void
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

    int arr[xsize][ysize] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 2, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 2, 0},
                             {0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0},
                             {0, 2, 0, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0},
                             {0, 0, 0, 2, 2, 2, 0, 2, 0, 2, 0, 2, 0, 2, 2, 2, 0, 0, 0},
                             {2, 0, 0, 0, 2, 3, 0, 2, 0, 2, 0, 2, 0, 3, 2, 0, 0, 0, 2},
                             {2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2},
                             {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
                             {2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 2, 2},
                             {0, 0, 0, 0, 0, 0, 0, 2, 7, 8, 9, 2, 0, 0, 0, 0, 0, 0, 0},
                             {2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 2, 2},
                             {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0},
                             {2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {2, 0, 0, 0, 2, 3, 0, 2, 0, 2, 0, 2, 0, 3, 2, 0, 0, 0, 0},
                             {0, 0, 0, 2, 2, 2, 0, 2, 0, 2, 0, 2, 0, 2, 2, 2, 0, 0, 0},
                             {0, 2, 0, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0},
                             {0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0},
                             {0, 2, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 2, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            if (arr[i][j] == 2) Map[i][j].setVoid();
            else if (arr[i][j] == 3) {
                Map[i][j] = pp;
            } else if (arr[i][j] == 0) {
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
        }

    }
    while (true) {
        srand(time(NULL));
        for (int i = 0; i < 10; i++) {
            rand();
        }
        int r1 = rand() % xsize;
        int r2 = rand() % ysize;
        TileData &tile = Map[r1][r2];
        if (tile.isVoid) continue;
        if (tile.entity != nullptr) continue;
        tile.entity = player;
        player->pos = {r1, r2};
        break;
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

bool comparePositions(Position &p1, const Position &p2) {
    if (p1.x == p2.x && p1.y == p2.y) return true;
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

    Position currPos(xpos, ypos);
    if ((entity->prevMove == 'a') && comparePositions(currPos, leftpass)) {
        newx = rightpass.x;
        newy = rightpass.y;
    } else if ((entity->prevMove == 'd') && comparePositions(currPos, rightpass)) {
        newx = leftpass.x;
        newy = leftpass.y;
    }

    if (newy > ysize - 1) {
        newy = 10;
    } else if (newy < 0) {
        newy = 0;
    } else if (newx > xsize - 1) {
        newx = 10;
    } else if (newx < 0) {
        newx = 0;
    }


    TileData *newLoc = &Map[newx][newy];

    //check collision
    if (newLoc->isVoid) {
        return;
    }
    if (!powerPellet) {
        if (newLoc->entity != nullptr) {
            for (int i = 0; i < 3; i++) {
                if (newLoc->entity == &enemy[i]) {
                    kill();
                }
            }
        }
    }
    if (powerPellet) {
        if (newLoc->entity != nullptr) {
            score += 100;
            for (int i = 0; i < 3; i++) {
                if (newLoc->entity == &enemy[i]) {
                    killEnemy(i);
                }
            }
        }
    }

    Map[xpos][ypos].entity = nullptr;
    newLoc->entity = entity;
    Position newPos(newx, newy);
    entity->pos = newPos;


    if (entity->id == "player") {
        if (newLoc->hasGarbage) {
            score += 10;
            newLoc->hasGarbage = false;
            remainingGarbage--;
            if (remainingGarbage == 0) {
                tickNum = 0;
                generateLevel();
            }
        }
        if (powerPellet) {
            remainingPPDuration--;
            if (remainingPPDuration < 0) {
                powerPellet = false;
                remainingPPDuration = 0;
            }
        }
        if (newLoc->hasPP) {
            score += 10;
            newLoc->hasPP = false;
            powerPellet = true;
            remainingPPDuration = 10;
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
            if (!powerPellet) {
                kill();
            }
            for (int i = 0; i < 3; i++) {
                if (&enemy[i] == entity) killEnemy(i);
            }
        }
        return;
    }
    Map[entity->pos.x][entity->pos.y].entity = nullptr;
    entity->pos = newpos;
    Map[entity->pos.x][entity->pos.y].entity = entity;
}

Position Game::predictPosition(Entity *entity, char prev) {
    Position currPos = entity->pos;
    Position newPos = entity->pos;
    int predLength = 4;
    if (abs(currPos.x - enemy[1].pos.x) + abs(currPos.y - enemy[1].pos.y)) {
        predLength = 0;
    }

    for (int i = 0; i < predLength; i++) {
        //adjust newPos
        if (entity->prevMove == 'w') {
            newPos.x--;
        } else if (entity->prevMove == 's') {
            newPos.x++;
        } else if (entity->prevMove == 'a') {
            newPos.y--;
        } else if (entity->prevMove == 'd') {
            newPos.y++;
        }

        //check bounds
        if (newPos.y > ysize - 1) {
            newPos.y = ysize - 1;
        } else if (newPos.y < 0) {
            newPos.y = 0;
        } else if (newPos.x > xsize - 1) {
            newPos.x = xsize - 1;
        } else if (newPos.x < 0) {
            newPos.x = 0;
        }

        if (Map[newPos.x][newPos.y].isVoid) {
            break;
        }
        currPos = newPos;
    }
    return currPos;
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
        entityPos[0] = player->pos;
    }

    if (tickNum % 4 == 0) {


        //first time spawns
        if (tickNum < 30) {
            if (tickNum == 4) {
                while (Map[outOfBox.x][outOfBox.y].entity != nullptr) {
                    tickNum++;
                    tick();
                    printMap();
                }
                Map[enemy[0].pos.x][enemy[0].pos.y].entity = nullptr;
                enemy[0].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[0];
                enemy[0].deathlength = 0;
                enemy[0].dead = false;
            } else if (tickNum == 16) {
                while (Map[outOfBox.x][outOfBox.y].entity != nullptr) {
                    tickNum++;
                    tick();
                    printMap();
                }
                Map[enemy[1].pos.x][enemy[1].pos.y].entity = nullptr;
                enemy[1].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[1];
                enemy[1].dead = false;
            } else if (tickNum == 28) {
                while (Map[outOfBox.x][outOfBox.y].entity != nullptr) {
                    tickNum++;
                    tick();
                    printMap();
                }
                Map[enemy[2].pos.x][enemy[2].pos.y].entity = nullptr;
                enemy[2].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[2];
                enemy[2].dead = false;
            }
        }

        //move enemies
        if (!enemy[0].dead) {
            vector<Position> path;
            path = a_star(enemy[0].pos, player->pos, Map);
            //vector is sorted start --> end
            if (!path.empty()) {
                moveEntity(&enemy[0], path[1]);
            }

            if (debug) {
                debugPath[0] = path;
                entityPos[1] = enemy[0].pos;
            }
        } else {
            enemy[0].deathlength++;
            if (enemy[0].deathlength > 10) {
                Map[enemy[0].pos.x][enemy[0].pos.y].entity = nullptr;
                enemy[0].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[0];
                enemy[0].deathlength = 0;
                enemy[0].dead = false;
            }
        }
        if (!enemy[1].dead) {
            vector<Position> path;

            path = a_star(enemy[1].pos, predictPosition(player, player->prevMove), Map);
            if (!path.empty()) {
                moveEntity(&enemy[1], path[1]);
            }


            if (debug) {
                debugPath[1] = path;
                entityPos[2] = enemy[1].pos;
            }

        } else {
            enemy[1].deathlength++;
            if (enemy[1].deathlength > 10) {
                Map[enemy[1].pos.x][enemy[1].pos.y].entity = nullptr;
                enemy[1].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[1];
                enemy[1].deathlength = 0;
                enemy[1].dead = false;
            }
        }
        if (!enemy[2].dead) {
            int distance = calcDistance(&enemy[2].pos, &player->pos);
            vector<Position> path;
            if (distance < 6) {
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
                        Position pos(enemy[2].pos.x + i, enemy[2].pos.y + j);
                        if (pos.x < 0 || pos.x >= xsize || pos.y + j < 0 || pos.y + j >= ysize) continue;
                        if (Map[pos.x][pos.y].isVoid) continue;
                        if (Map[pos.x][pos.y].entity != nullptr) continue;
                        possible.emplace_back(enemy[2].pos.x + i, enemy[2].pos.y + j);
                    }
                }
                if (!possible.empty()) {
                    int random = rand() % int(possible.size());
                    if (debug) {
                        path = vector<Position>(2);
                        path[0] = enemy[2].pos;
                        path[1] = possible[random];
                    }
                    moveEntity(&enemy[2], possible[random]);
                } else if (debug) {
                    path = vector<Position>(1);
                    path[0] = enemy[2].pos;
                }

            }
            if (debug) {
                debugPath[2] = path;
                entityPos[3] = enemy[0].pos;
            }
        } else {
            enemy[2].deathlength++;
            if (enemy[2].deathlength > 10) {
                Map[enemy[2].pos.x][enemy[0].pos.y].entity = nullptr;
                enemy[2].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[2];
                enemy[2].deathlength = 0;
                enemy[2].dead = false;
            }
        }
    }
    tickNum++;
}

void Game::kill() {
    system("clear");
    cout << "You Died!\n========\nYour Score: " << score << "\n";
    this_thread::sleep_for(chrono::seconds(2));
    //TODO: add endscreen (render)
    endwin(); // end ncurses mode
    exit(1);
}

void Game::killEnemy(int i) {
    Entity *curr = &enemy[i];
    score += (i + 1) * 1000;
    curr->dead = true;
    curr->deathlength = 0;
    Map[curr->pos.x][curr->pos.y].entity = nullptr;
    TileData *currTile;
    int x, y;
    for (i = 0; i < 3; i++) {
        x = enemyBox[i].x;
        y = enemyBox[i].y;
        currTile = &Map[x][y];
        if (currTile->entity != nullptr) continue;
        break;
    }
    currTile->entity = curr;
    curr->pos = {x, y};
}
