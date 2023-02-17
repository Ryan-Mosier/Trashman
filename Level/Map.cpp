//
// Created by ryanm on 2/16/2023.
//

#include "Map.h"
#include <iostream>

using namespace std;

Game::Game() {
    generateLevel();
}

void Game::printMap() {
    for (auto &i: Map) {
        for (TileData j: i) {
            if (j.isVoid) {
                cout << " # ";
            } else if (!j.isVoid) {
                cout << "   ";
            }
        }
        cout << "\n";
    }
}

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

    int arr[xsize][ysize] = {{0,  0,  0,  0,  0, 0, 0,  0,  0,  0},
                             {0,  -1, -1, -1, 0, 0, -1, -1, -1, 0},
                             {0,  0,  -1, 0,  0, 0, 0,  -1, 0,  0},
                             {0,  0,  0,  0,  0, 0, 0,  0,  0,  0},
                             {-1, -1, 0,  -1, 0, 0, -1, 0,  -1, -1},
                             {-1, -1, 0,  -1, 0, 0, -1, 0,  -1, -1},
                             {0,  0,  0,  0,  0, 0, 0,  0,  0,  0},
                             {0,  0,  -1, 0,  0, 0, 0,  -1, 0,  0},
                             {0,  -1, -1, -1, 0, 0, -1, -1, -1, 0},
                             {0,  0,  0,  0,  0, 0, 0,  0,  0,  0}};
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