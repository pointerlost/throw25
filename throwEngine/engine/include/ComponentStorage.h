//
// Created by pointerlost on 8/6/25.
//
#pragma once
#include <iostream>
#include <unordered_map>
#include "Entity.h"


template<typename T>
class ComponentStorage {
public:
    void add(Entity e, T comp);
    void remove(Entity e);
    T *get(Entity e);
    auto &getAll();

private:
    std::unordered_map<Entity, T> components;
};
