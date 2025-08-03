#pragma once
#include <graphics/Mesh/GLMesh3D.h>
#include "graphics/Mesh/GLMeshData3D.h"


namespace GLgraphics
{
	// Forward Declarations
	struct Vertex;
	struct MeshData3D;

	class MeshCircle3D : public Mesh
	{
	public:
		MeshCircle3D(const std::shared_ptr<MeshData3D>& data, const SubMeshInfo& info);
		~MeshCircle3D() = default;

		void drawMeshObject() const override;

		uint32_t getVAO() const override { return VAO; };

	protected:

		void SetUpMeshResources() override;

	private:
		const std::shared_ptr<MeshData3D> meshData3D;

		SubMeshInfo info;

		uint32_t VAO;
	};
}