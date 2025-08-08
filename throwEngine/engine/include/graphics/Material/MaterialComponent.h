//
// Created by pointerlost on 8/8/25.
//
#pragma once
#include <memory>

namespace Graphics {
    struct Texture;
}

namespace Graphics {
    struct Material;
}

struct MaterialComponent {
    std::shared_ptr<Graphics::Material> material;
    std::shared_ptr<Graphics::Texture> texture;
};
