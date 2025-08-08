//
// Created by pointerlost on 8/8/25.
//
#pragma once
#include <memory>


namespace Graphics {
    class Camera;
};

struct CameraComponent {
    std::shared_ptr<Graphics::Camera> camera;
};