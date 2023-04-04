//
// Created by ryanm on 3/31/2023.
//

#include "Structures.h"

void TileData::setVoid() {
    isVoid = true;
    entity = nullptr;
    hasGarbage = false;
    hasRecycling = false;
}

void TileData::setEmpty() {
    isVoid = false;
    entity = nullptr;
    hasGarbage = true;
    hasRecycling = false;
}

AStarNode::AStarNode() {}

AStarNode::AStarNode(TileData &tileData) {
    position = tileData.pos;
}