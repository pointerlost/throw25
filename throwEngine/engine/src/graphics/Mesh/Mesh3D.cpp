#include "graphics/Mesh/Mesh3D.h"

#include "graphics/Mesh/MeshData3D.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Graphics
{

	Mesh::Mesh(const std::shared_ptr<MeshData3D>& data) :
		meshData(data)
	{
		// SET UP MESH RESOURCES
		SetUpMeshResources();
	}

	void Mesh::SetUpMeshResources()
	{
		const auto& vertices = meshData->getVertices();
		const auto& indices = meshData->getIndices();

		std::cout << "vertices size: " << vertices.size() << "\n";
		std::cout << "indices size: " << indices.size() << "\n";

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// set wireframe mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

		meshData->setVBO(VBO);
		meshData->setEBO(EBO);
	}

	void Mesh::drawMeshObject() const
	{
		std::cout << "There is nothing to override from base because just keeps data about meshes!\n";
	}

}