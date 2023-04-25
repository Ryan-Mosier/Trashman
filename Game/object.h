//
// Created by ryanm on 4/19/2023.
//

#ifndef MYPROJECT_OBJECT_H
#define MYPROJECT_OBJECT_H

#include "Structures.h"
#include <unordered_map>
#include "objectHashTable.h"

class objectHandler {
private:
    objectHashTable hashTable;

public:
    object *head;

    objectHandler();

    ~objectHandler();

    void insert(object *obj);

    void remove(object *obj);

    bool move(Position &old, Position &newPos, type);

    bool move(const Position &, const Position &, type);

    object *find(Position &pos, type);
};


#endif //MYPROJECT_OBJECT_H
