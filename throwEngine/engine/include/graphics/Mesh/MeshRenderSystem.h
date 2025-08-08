//
// Created by pointerlost on 8/6/25.
//
#pragma once
#include <iostream>

// forward declarations
class World;
namespace Graphics { class Camera; }
struct MeshComponent;

namespace Render {
    class MeshRenderSystem {
    public:
        void render(const World& world, const Graphics::Camera& camera);

    private:
        void setupVAO(MeshComponent& comp);
    };

}

