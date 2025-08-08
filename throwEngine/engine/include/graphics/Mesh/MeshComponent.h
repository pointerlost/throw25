//
// Created by pointerlost on 8/6/25.
//
#pragma once
#include <memory>

// add include for mesh data 3d if needed

namespace Graphics {
    class MeshData3D;
}

struct MeshComponent {
    std::shared_ptr<Graphics::MeshData3D> meshData;
    std::string subMeshName;
    uint32_t VAO;
};
