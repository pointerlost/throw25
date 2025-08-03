#pragma once
#include <memory>
#include "graphics/Mesh/GLMeshInterface.h"

namespace GLgraphics
{
	// Forward Declarations
	class Shader;
	class MeshData3D;

	class Mesh : public IMesh {
	public:

		// Constructor
		explicit Mesh(const std::shared_ptr<MeshData3D>& data);

		// Default-Constructor and destructor
		Mesh() = default;
		~Mesh() = default;

		[[nodiscard]] uint32_t getVBO() const { return VBO; };
		[[nodiscard]] uint32_t getEBO() const { return EBO; };

		void drawMeshObject() const override;

		[[nodiscard]] uint32_t getVAO() const override { return 0; };

	protected:

		void SetUpMeshResources() override;

	private:
		std::shared_ptr<MeshData3D> meshData;
		
		uint32_t VBO = 0, EBO = 0;
	};

}
