//
// Created by ryanm on 4/20/2023.
//

#include "objectHashTable.h"

objectHashTable::objectHashTable() {
    for (int i = 0; i < sizeof(table); i++) {
        table[0][i] = nullptr;
    }
}

objectHashTable::~objectHashTable() {}

void objectHashTable::insert(object *obj) {
    Position pos = obj->pos;
    hashNode *newNode = new hashNode(obj), *curr = table[pos.x][pos.y];
    newNode->next = curr;
    table[pos.x][pos.y] = newNode;
}

void objectHashTable::remove(object *obj) {
    Position pos = obj->pos;
    hashNode *curr = table[pos.x][pos.y];
    hashNode *prev = nullptr;
    while (curr != nullptr) {
        if (curr->obj == obj) {
            if (prev != nullptr) {
                prev->next = curr->next;
                delete curr;
                return;
            }
            table[pos.x][pos.y] = curr->next;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

bool objectHashTable::move(object *obj, Position *old, Position *newPos) {
    obj->pos = *old;
    remove(obj);
    obj->pos = *newPos;
    insert(obj);
    return false;
}

object *objectHashTable::find(Position &pos, type t) {
    hashNode *curr = table[pos.x][pos.y];
    while (curr != nullptr) {
        if (curr->obj->obj == t) return curr->obj;
        curr = curr->next;
    }
    return nullptr;
}
