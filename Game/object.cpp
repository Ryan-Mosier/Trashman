//
// Created by ryanm on 4/19/2023.
//

#include "object.h"

objectHandler::objectHandler() : head(nullptr) {

}

objectHandler::~objectHandler() {
    object *curr = head;
    object *prev;
    while (curr != nullptr) {
        prev = curr;
        curr = curr->next;
        delete prev;
    }
}

void objectHandler::insert(object *obj) {
    hashTable.insert(obj);
    obj->next = head;
    if (head != nullptr) head->prev = obj;
    head = obj;
}

void objectHandler::remove(object *obj) {
    if (obj->prev != nullptr) obj->prev->next = obj->next;
    if (obj->next != nullptr) obj->next->prev = obj->prev;
    hashTable.remove(obj);
    delete obj;
}

bool objectHandler::move(Position &old, Position &newPos, type t) {
    object *obj = hashTable.find(old, t);
    hashTable.move(obj, &old, &newPos);
    return true;
}

bool objectHandler::move(const Position &old, const Position &newPos, type t) {
    Position temp = old, temp2 = newPos;
    object *obj = hashTable.find(temp, t);
    hashTable.move(obj, &temp, &temp2);
    return true;
}

object *objectHandler::find(Position &pos, type t) {
    return hashTable.find(pos, t);
}
