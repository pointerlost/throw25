#include "graphics/Mesh/GLMeshCircle3D.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace GLgraphics
{

	MeshCircle3D::MeshCircle3D(const std::shared_ptr<MeshData3D>& data, const SubMeshInfo &info) :
		meshData3D(data), info(info)
	{
		/* do it some stuff! */
		if (info.VAO != 0) {
			this->VAO = info.VAO;
		}
		else {
			SetUpMeshResources();
		}
	}

	void MeshCircle3D::drawMeshObject() const
	{
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, info.indexCount, GL_UNSIGNED_INT, (void*)(info.indexOffset * sizeof(uint32_t)));
		// why are we using GL_TRIANGLES, we can change it another time!!! need update!
		
	}

	void MeshCircle3D::SetUpMeshResources()
	{
		glGenVertexArrays(1, &info.VAO);
		glBindVertexArray(info.VAO);

		this->VAO = info.VAO;

		glBindBuffer(GL_ARRAY_BUFFER, meshData3D->getVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData3D->getEBO());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	}

}