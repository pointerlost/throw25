//
// Created by pointerlost on 8/6/25.
//
#include "graphics/Mesh/MeshRenderSystem.h"

#include "World.h"
#include "graphics/Camera/Camera.h"
#include "graphics/Mesh/Mesh3D.h"
#include "graphics/Mesh/MeshData3D.h"
#include "graphics/Mesh/MeshComponent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Render
{
    void MeshRenderSystem::render(const World &world, const Graphics::Camera &camera) {
    }

    void MeshRenderSystem::setupVAO(MeshComponent &comp) {
        if (comp.VAO != 0) return;

        auto& subMesh = comp.meshData->getObjectInfo(comp.subMeshName);

        glGenVertexArrays(1, &comp.VAO);
        glBindVertexArray(comp.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, comp.meshData->getVBO());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comp.meshData->getEBO());

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Graphics::Vertex), reinterpret_cast<void *>(offsetof(Graphics::Vertex, position)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Graphics::Vertex), reinterpret_cast<void *>(offsetof(Graphics::Vertex, normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Graphics::Vertex), reinterpret_cast<void *>(offsetof(Graphics::Vertex, color)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Graphics::Vertex), reinterpret_cast<void *>(offsetof(Graphics::Vertex, texCoords)));

        // Close VAO
        glBindVertexArray(0);

        subMesh.VAO = comp.VAO;
    }
}
