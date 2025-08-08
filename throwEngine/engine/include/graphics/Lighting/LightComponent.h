//
// Created by pointerlost on 8/8/25.
//
#pragma once
#include <memory>

namespace LIGHTING {
    class Light;
    class LightData;
}

struct LightComponent {
    std::shared_ptr<LIGHTING::Light> light;
};