//
// Created by ryanm on 3/31/2023.
//

#include "Structures.h"
#include <limits.h>

void TileData::setVoid() {
    isVoid = true;
    entity = nullptr;
    hasGarbage = false;
    hasPP = false;
}

void TileData::setEmpty() {
    isVoid = false;
    entity = nullptr;
    hasGarbage = true;
    hasPP = false;
}