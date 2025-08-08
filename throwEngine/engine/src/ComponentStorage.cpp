//
// Created by pointerlost on 8/7/25.
//
#include "ComponentStorage.h"
#include "core/Logger.h"

template<typename T>
void ComponentStorage<T>::add(Entity e, T comp) {
    if (components.contains(e)) {
        Logger::warn("Trying to add component that was already added");
        return;
    }
    components[e] = comp;
}

template<typename T>
void ComponentStorage<T>::remove(Entity e) {
    if (components.contains(e)) {
        components.erase(e);
    }
}

template<typename T>
T * ComponentStorage<T>::get(Entity e) {
    if (!components.contains(e)) {
        return {};
    }
    return &components[e];
}

template<typename T>
auto & ComponentStorage<T>::getAll() {
    return components;
}
