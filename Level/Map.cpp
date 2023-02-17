//
// Created by ryanm on 2/16/2023.
//

#include "Map.h"
#include <iostream>

using namespace std;


void TileData::setVoid() {
    isVoid = true;
    hasPlayer = false;
    hasEnemy = false;
    hasGarbage = false;
    hasRecycling = false;
}

void TileData::setEmpty() {
    isVoid = false;
    hasPlayer = false;
    hasEnemy = false;
    hasGarbage = false;
    hasRecycling = false;
}

void TileData::setPlayer() {
    isVoid = false;
    hasPlayer = true;
    hasEnemy = false;
    hasGarbage = false;
    hasRecycling = false;
}


Game::Game() {
    generateLevel();
}

void Game::printMap() {
    cout << "#  #  #  #  #  #  #  #  #  #  #  #  #\n";
    for (auto &i: Map) {
        for (int j = -1; j < xsize + 1; j++) {
            if (j != -1 && j != xsize) {
                if (i[j].isVoid) {
                    cout << " # ";
                }
                if (!i[j].isVoid) {
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
    score += 100;
    // -1 = void
    TileData voidTile;
    voidTile.setVoid();
    // 0 = empty
    TileData emptyTile;
    emptyTile.setEmpty();
    //
    //

    int arr[xsize][ysize] = {{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
                             {0,  -1, -1, -1, -1, 0,  -1, -1, -1, -1, 0},
                             {0,  0,  -1, 0,  -1, 0,  -1, 0,  -1, 0,  0},
                             {-1, 0,  0,  0,  0,  0,  0,  0,  0,  0,  -1},
                             {-1, -1, 0,  -1, -1, -1, -1, -1, 0,  -1, -1},
                             {0,  0,  0,  -1, 0,  0,  0,  -1, 0,  0,  0},
                             {-1, -1, 0,  -1, -1, -1, -1, -1, 0,  -1, -1},
                             {-1, 0,  0,  0,  0,  0,  0,  0,  0,  0,  -1},
                             {0,  0,  -1, 0,  -1, 0,  -1, 0,  -1, 0,  0},
                             {0,  -1, -1, -1, -1, 0,  -1, -1, -1, -1, 0},
                             {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}};
//    int arr[10][10] = {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//                             { 0,-1,-1,-1, 0, 0,-1,-1,-1, 0},
//                             { 0, 0,-1, 0, 0, 0, 0,-1, 0, 0},
//                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//                             {-1,-1, 0,-1, 0, 0,-1, 0,-1,-1},
//                             {-1,-1, 0,-1, 0, 0,-1, 0,-1,-1},
//                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//                             { 0, 0,-1, 0, 0, 0, 0,-1, 0, 0},
//                             { 0,-1,-1,-1, 0, 0,-1,-1,-1, 0},
//                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            if (arr[i][j] == -1) {
                Map[i][j] = voidTile;
            } else if (arr[i][j] == 0) {
                Map[i][j] = emptyTile;
            }
        }

    }
}

void Game::moveEntity(Entity *entity) {
//TODO: move enemies
}
