#include "graphics/Mesh/GLMeshCube3D.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GLgraphics
{

	MeshCube3D::MeshCube3D(const std::shared_ptr<MeshData3D>& data, const SubMeshInfo &info)
		: meshData3D(data), info(info)
	{
		/* do it some stuff! */
		if (info.VAO != 0) {
			this->VAO = info.VAO;
		}
		else {
			SetUpMeshResources();
		}
	}

	void MeshCube3D::drawMeshObject() const
	{
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, info.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void *>(info.indexOffset * sizeof(uint32_t)));
		// why are we using GL_TRIANGLES, we can change it another time!!! need update!
	}

	void MeshCube3D::SetUpMeshResources()
	{
		// Create and Bind VAO
		glGenVertexArrays(1, &info.VAO);
		glBindVertexArray(info.VAO);

		this->VAO = info.VAO;

		glBindBuffer(GL_ARRAY_BUFFER, meshData3D->getVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData3D->getEBO());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position)));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, color)));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

		// Close vao
		glBindVertexArray(0);
	}

}