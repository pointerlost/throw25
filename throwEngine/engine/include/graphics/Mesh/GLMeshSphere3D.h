#pragma once
#include "graphics/Mesh/GLMesh3D.h"

#include "graphics/Mesh/GLMeshData3D.h"

namespace GLgraphics {

	class MeshSphere3D : public Mesh
	{
	public:
		MeshSphere3D(std::shared_ptr<MeshData3D> data, const SubMeshInfo& info);
		~MeshSphere3D() = default;

		void drawMeshObject() const override;

		uint32_t getVAO() const override { return VAO; };

	protected:

		void SetUpMeshResources() override;

	private:
		std::shared_ptr<MeshData3D> meshData3D;
		SubMeshInfo info;

		uint32_t VAO;
	};
}