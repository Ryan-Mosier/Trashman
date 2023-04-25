//
// Created by ryanm on 2/16/2023.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <ncurses.h>
#include <random>
#include <unordered_set>
#include "Game.h"
#include "colors.h"
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include "aStar.h"
#include "debugValues.h"
#include "object.h"


using namespace std;

void fullExit() {
    system("clear");
    cout << "EXITING\n";
    exit(42);
}

gameLogic::gameLogic(GameState *gs) : State(gs), pathfinding(/*#region arr*/
        {{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
         {2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2},
         {2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2},
         {2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2},
         {0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0},
         {2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2},
         {2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 2, 2, 2},
         {2, 2, 2, 0, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 2, 2, 2},
         {2, 2, 2, 0, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 2, 2, 0, 2, 9, 2},
         {2, 7, 2, 0, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 2, 2, 0, 2, 8, 2},
         {2, 2, 2, 0, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 2, 2, 2},
         {2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 2, 2, 2},
         {2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2},
         {0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0},
         {2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2},
         {2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2},
         {2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2},
         {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        }
        /*#endregion*/) {
    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            Map[j][i] = TileData(j, i);
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
    /*#region Map*/

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

    std::vector<std::vector<int>> arr =
            {{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
             {2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2},
             {2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2},
             {2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2},
             {0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0},
             {2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2},
             {2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 2, 2, 2},
             {2, 2, 2, 0, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 2, 2, 2},
             {2, 2, 2, 0, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 2, 2, 0, 2, 9, 2},
             {2, 7, 2, 0, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 0, 2, 2, 0, 2, 8, 2},
             {2, 2, 2, 0, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 2, 2, 2},
             {2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 2, 2, 2},
             {2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2},
             {0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0},
             {2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2},
             {2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2},
             {2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2},
             {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
            };

    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            if (arr[j][i] == 2) Map[j][i].setVoid();
            else if (arr[j][i] == 3) {
                Map[j][i] = pp;
                object *node = new object(type::pp, {i, j}, null);
                objecthandler.insert(node);
            } else if (arr[j][i] == 0) {
                Map[j][i].setEmpty();
                remainingGarbage++;
                object *node = new object(type::pellet, {i, j}, null);
                objecthandler.insert(node);
            } else if (arr[j][i] == 1) {
                Map[j][i].setEmpty();
                Map[j][i].entity = player;
                Map[j][i].entity->pos = Map[j][i].pos;
                object *node = new object(type::player, {i, j}, state::left);
                objecthandler.insert(node);
            } else if (arr[j][i] == 7) {
                Map[j][i].setEmpty();
                Map[j][i].entity = &enemy[0];
                Map[j][i].entity->pos = Map[j][i].pos;
                object *node = new object(type::ghost_red, {i, j}, state::right);
                objecthandler.insert(node);
            } else if (arr[j][i] == 8) {
                Map[j][i].setEmpty();
                Map[j][i].entity = &enemy[1];
                Map[j][i].entity->pos = Map[j][i].pos;
                object *node = new object(type::ghost_pink, {i, j}, state::left);
                objecthandler.insert(node);
            } else if (arr[j][i] == 9) {
                Map[j][i].setEmpty();
                Map[j][i].entity = &enemy[2];
                Map[j][i].entity->pos = Map[j][i].pos;
                object *node = new object(type::ghost_yellow, {i, j}, state::left);
                objecthandler.insert(node);
            }
        }
    }
    while (true) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis_x(0, xsize - 1);
        std::uniform_int_distribution<> dis_y(0, ysize - 1);

        int r1 = dis_x(gen);
        int r2 = dis_y(gen);
        TileData &tile = Map[r2][r1];
        if (tile.isVoid) continue;
        if (tile.hasPP) continue;
        if (tile.entity != nullptr) continue;
        tile.hasGarbage = false;
        tile.entity = player;
        player->pos = {r1, r2};
        while (objecthandler.find(player->pos, type::pellet) != nullptr) {
            object *temp = objecthandler.find(player->pos, type::pellet);
            objecthandler.remove(temp);
        }
        auto *node = new object(type::player, player->pos, state::left);
        objecthandler.insert(node);
        break;
    }

    /*#endregion*/
    this->recentInput = 'w';
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

gameLogic::~gameLogic() {
    delete[] enemy;
    delete player;
}

void gameLogic::DebugprintMap() {
    system("clear");
    for (int i = 0; i < ysize; i++) {
        for (int j = 0; j < xsize; j++) {
            TileData &tile = Map[i][j];
            if (tile.isVoid) {
                cout << " # ";
            } else if (tile.entity != nullptr) {
                if (tile.entity->id == "player") {
                    cout << "\033[33m"; //YELLOW
                } else if (tile.entity->id == "1") {
                    cout << "\033[31m"; //RED
                } else if (tile.entity->id == "2") {
                    cout << "\033[35m"; //MAGENTA
                } else if (tile.entity->id == "3") {
                    cout << "\033[36m"; //CYAN
                }
                cout << " @ ";
                cout << "\033[0m";
            } else if (!tile.isVoid) {
                if (!tile.hasGarbage) {
                    cout << "   ";
                } else {
                    cout << "   ";
//                    cout << "\033[33m";
//                    cout << " * ";
//                    cout << "\033[0m";
                }

            }
        }
        cout << "\n";
    }
}

void gameLogic::printMap() {
    DebugprintMap();
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

void gameLogic::moveEntity(Entity *entity, char curr) {
    int xpos = entity->pos.x, ypos = entity->pos.y;


    if (validateCharMov(curr)) {
        entity->prevMove = curr;
    }
    if (!validateCharMov(entity->prevMove)) {
        return;
    }

    Position currPos(xpos, ypos);
    object *node = objecthandler.find(currPos, type::player);
    if (node == nullptr) exit(-1);

    int newx = xpos, newy = ypos;
    if (entity->prevMove == 'w') {
        newy--;
        node->direction = up;
    } else if (entity->prevMove == 's') {
        newy++;
        node->direction = down;
    } else if (entity->prevMove == 'a') {
        newx--;
        node->direction = state::left;
    } else if (entity->prevMove == 'd') {
        newx++;
        node->direction = state::right;
    }


    //TODO: make for new passes
    if ((entity->prevMove == 'a') && comparePositions(currPos, topLeftPass)) {
        newx = topRightPass.x;
        newy = topRightPass.y;
    } else if ((entity->prevMove == 'd') && comparePositions(currPos, topRightPass)) {
        newx = topLeftPass.x;
        newy = topLeftPass.y;
    }

    TileData *oldLoc = &Map[ypos][xpos];
    TileData *newLoc = &Map[newy][newx];

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

    oldLoc->entity = nullptr;
    newLoc->entity = entity;
    Position newPos(newx, newy);
    entity->pos = newPos;

    Position oldPos(xpos, ypos);

    if (entity->id == "player") {
        if (newLoc->hasGarbage) {
            score += 10;
            newLoc->hasGarbage = false;
            remainingGarbage--;
            object *pellet = objecthandler.find(newPos, type::pellet);
            if (pellet != nullptr) objecthandler.remove(pellet);
            if (remainingGarbage == 0) {
                //TODO: win
                tickNum = 0;
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
            objecthandler.remove(objecthandler.find(oldPos, type::pp));
        }
        objecthandler.move(oldPos, newPos, type::player);
    }

}

state findDirection(Position p1, Position p2) {
    if (p1.x != p2.x) {
        if (p1.x > p2.x) {
            return state::left;
        }
        return state::right;
    }
    if (p1.y > p2.y) {
        return up;
    }
    return down;
}

void gameLogic::moveEntity(Entity *entity, Position newpos) {
    int distance = abs(entity->pos.x - newpos.x) + abs(entity->pos.y - newpos.y);
    type typeOfEnemy = ghost_yellow;
    object *temp = objecthandler.find(entity->pos, typeOfEnemy);
    if (temp == nullptr) {
        typeOfEnemy = ghost_pink;
        temp = objecthandler.find(entity->pos, typeOfEnemy);
    }
    if (temp == nullptr) {
        typeOfEnemy = ghost_red;
        temp = objecthandler.find(entity->pos, typeOfEnemy);
    }
    temp->direction = findDirection(entity->pos, newpos);
    if (distance > 1) {
        return;
    }
    if (newpos == player->pos) {
        if (!powerPellet) {
            kill();
        }
        for (int i = 0; i < 3; i++) {
            if (&enemy[i] == entity) killEnemy(i);
        }
        return;
    }
    if (Map[newpos.x][newpos.y].entity != nullptr) {
        return;
    }
    Map[entity->pos.x][entity->pos.y].entity = nullptr;
    objecthandler.move(entity->pos, newpos, typeOfEnemy);
    entity->pos = newpos;
    Map[entity->pos.x][entity->pos.y].entity = entity;
}

Position gameLogic::predictPosition(Entity *entity, char prev) {
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

void gameLogic::tick() {
    char in = '\0';
    if (tickNum == 0) {
        in = this->recentInput;
        moveEntity(player, in);
    } else if (tickNum % 2 == 0) {
        in = this->recentInput;
        moveEntity(player, in);
        if (debug) {
            ::entityPos = std::vector<Position>{player->pos};
        }
    }

    if (tickNum % 4 == 0) {

        //first time spawns
        if (tickNum < 30) {
            if (tickNum == 4) {
                while (Map[outOfBox.x][outOfBox.y].entity != nullptr) {
                    tickNum++;
                    tick();

                }
                Map[enemy[0].pos.x][enemy[0].pos.y].entity = nullptr;
                object *ghost = objecthandler.find(enemy[0].pos, type::ghost_red);
                objecthandler.move(enemy[0].pos.reverse(), outOfBox, type::ghost_red);
                enemy[0].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[0];
                enemy[0].deathlength = 0;
                enemy[0].dead = false;
            } else if (tickNum == 16) {
                while (Map[outOfBox.x][outOfBox.y].entity != nullptr) {
                    tickNum++;
                    tick();

                }
                Map[enemy[1].pos.x][enemy[1].pos.y].entity = nullptr;
                objecthandler.move(enemy[1].pos.reverse(), outOfBox, type::ghost_pink);
                enemy[1].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[1];
                enemy[1].dead = false;
            } else if (tickNum == 28) {
                while (Map[outOfBox.x][outOfBox.y].entity != nullptr) {
                    tickNum++;
                    tick();

                }
                Map[enemy[2].pos.x][enemy[2].pos.y].entity = nullptr;
                objecthandler.move(enemy[2].pos.reverse(), outOfBox, type::ghost_yellow);
                enemy[2].pos = outOfBox;
                Map[outOfBox.x][outOfBox.y].entity = &enemy[2];
                enemy[2].dead = false;
            }
        }

        //move enemies
        if (!enemy[0].dead) {
            vector<Position> path;
            path = pathfinding.aStar(enemy[0].pos, player->pos);
            //vector is sorted start --> end
            if (!path.empty()) {
                moveEntity(&enemy[0], path[1]);
            } else {
                cerr << "Path Empty!\n";
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

            path = pathfinding.aStar(enemy[1].pos, predictPosition(player, player->prevMove));
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

            int dis = distance(enemy[2].pos, player->pos);
            vector<Position> path;
            if (dis < 6) {
                path = pathfinding.aStar(enemy[2].pos, player->pos);
                //vector is sorted start --> end
                if (!path.empty()) {
                    moveEntity(&enemy[2], path[1]);
                }

            } else {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dist;
                int value = dist(gen);
                pathfinding.random(enemy[2].pos, value);
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

void gameLogic::kill() {
    *State = GameState::dead;
}

void gameLogic::killEnemy(int i) {
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
