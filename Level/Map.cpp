//
// Created by ryanm on 2/16/2023.
//

#include "Map.h"
#include "colors.h"
#include <iostream>
#include <cstdlib>

using namespace std;


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

void TileData::setPlayer() {
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


    system("clear");
    cout << "#  #  #  #  #  #  #  #  #  #  #  #  #\n";
    for (auto &i: Map) {
        for (int j = -1; j < xsize + 1; j++) {
            if (j != -1 && j != xsize) {
                if (i[j].isVoid) {
                    cout << " # ";
                } else if (i[j].entity != nullptr) {
                    if (i[j].entity->id == "player") {
                        cout << YELLOW;
                    } else if (i[j].entity->id == "1") {
                        cout << RED;
                    } else if (i[j].entity->id == "2") {
                        cout << MAGENTA;
                    } else if (i[j].entity->id == "3") {
                        cout << CYAN;
                    }
                    cout << " @ " << RESET;
                } else if (!i[j].isVoid) {
                    cout << "   ";
                }
            } else {
                if (j == -1) cout << "# ";
                else cout << " #";
            }
        }
        cout << endl;
    }
    cout << "#  #  #  #  #  #  #  #  #  #  #  #  #\n";
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

    int arr[xsize][ysize] = {{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                             {0,  -1, -1, -1, -1, 0,  -1, -1, -1, -1, 0},
                             {0,  0,  -1, 0,  -1, 0,  -1, 0,  -1, 0,  0},
                             {-1, 0,  0,  0,  0,  0,  0,  0,  0,  0,  -1},
                             {-1, -1, 0,  -1, -1, -1, -1, -1, 0,  -1, -1},
                             {0,  0,  1,  -1, 7,  8,  9,  -1, 0,  0,  0},
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

    if (entity->id != "player") {
        //TODO:pathfinding and generate direction
    }

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
    if(entity->id != "player"){
        if(Map[newx][newy].entity->id == "player"){
            //TODO: CALL END FUNCTION AND
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

    //TODO: collisionlogic
}


void Game::tick() {
    //TODO: tick function
}

void Game::movePlayer(char input) {
    moveEntity(player, input);
}

void Game::kill() {
    exit(score);
    //TODO: print endscreen and exit function
}
