//
// Created by ryanm on 4/20/2023.
//

#ifndef MYPROJECT_OBJECTHASHTABLE_H
#define MYPROJECT_OBJECTHASHTABLE_H

#include "Structures.h"

enum state {
    up,
    down,
    left,
    right,
    null
};

enum type {
    ghost_red,
    ghost_yellow,
    ghost_pink,
    player,
    pp,
    pellet
};

struct object {
    object *next, *prev;
    Position pos;
    type obj;
    state direction;

    object(enum type s, Position pos, enum state st) : pos(pos), obj(s), direction(st) {}
};

struct hashNode {
    object *obj;
    hashNode *next;

    hashNode(object *obj) : obj(obj) {}
};

class objectHashTable {
private:
    hashNode *table[xsize][ysize];

public:
    objectHashTable();

    ~objectHashTable();

    void insert(object *obj);

    void remove(object *obj);

    bool move(object *oldObj,Position *old,Position *newPos);

    object *find(Position &pos,type);
};


#endif //MYPROJECT_OBJECTHASHTABLE_H
