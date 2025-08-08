//
// Created by pointerlost on 8/7/25.
//
#pragma once
#include <memory>

class Transform;

struct TransformComponent {
    std::shared_ptr<Transform> transform;
};
