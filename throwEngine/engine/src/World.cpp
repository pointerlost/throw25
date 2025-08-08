//
// Created by pointerlost on 8/7/25.
//
#include "World.h"
#include "ComponentStorage.h"
#include "graphics/Camera/CameraComponent.h"
#include "graphics/Lighting/LightComponent.h"
#include "graphics/Transformations/TransformComponent.h"
#include "graphics/Mesh/MeshComponent.h"
#include "graphics/Material/MaterialComponent.h"

void World::deleteEntity(Entity e) {
    // Delete each component one by one (this is a c++ problem but can be tried later)
    getStorage<TransformComponent>().remove(e);
    getStorage<MeshComponent>().remove(e);
    getStorage<MaterialComponent>().remove(e);
    getStorage<LightComponent>().remove(e);
    getStorage<CameraComponent>().remove(e);
}

template<typename T>
void World::addComponent(Entity e, T c) {
    getStorage<T>().add(e, std::move(c));
}

template<typename T>
T *World::getComponent(Entity e) {
    return getStorage<T>().get(e);
}

template<typename T>
ComponentStorage<T> &World::getStorage() {
    static ComponentStorage<T> storage;
    return storage;
}

Entity World::createEntity() {
    return nextID++;
}
