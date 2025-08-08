//
// Created by pointerlost on 8/6/25.
//
#pragma once
#include <iostream>
#include <queue>
#include <unordered_map>

#include "ComponentStorage.h"
#include "Entity.h"

enum struct ComponentTypes {
    TransformComponent,
    MeshComponent,
    MaterialComponent,
    LightComponent,
    CameraComponent
};

class World {
public:
    Entity createEntity();

    void deleteEntity(Entity e);

    template<typename T>
    void addComponent(Entity e, T c);

    template<typename T>
    T* getComponent(Entity e);

    template<typename T>
    ComponentStorage<T>& getStorage();

private:
    Entity nextID = 1;
};
