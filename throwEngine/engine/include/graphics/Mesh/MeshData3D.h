#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>


namespace Graphics {

	struct Vertex
	{
	public:
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 texCoords;
	};

	struct SubMeshInfo
	{
		uint32_t vertexOffset;
		uint32_t indexOffset;
		uint32_t vertexCount;
		uint32_t indexCount;

		// VAO ownership
		uint32_t VAO;
	};

	class MeshData
	{
	public:
		MeshData() = default;

		SubMeshInfo& AddMesh(std::vector<Vertex> v, std::vector<uint32_t> i);
		void AddSubMeshInfo(const SubMeshInfo s);

		uint32_t getVBO() const { return VBO; };
		uint32_t getEBO() const { return EBO; };

		void setVBO(uint32_t vbo) { VBO = vbo; };
		void setEBO(uint32_t ebo) { EBO = ebo; };

		// setter & getter methods
		const std::vector<SubMeshInfo>& getSubMeshInfos() const { return subMeshInfos; };
		const std::vector<Vertex>& getVertices() const { return all_Vertices; };
		const std::vector<unsigned int>& getIndices() const { return all_Indices; };

	protected:
		std::vector<Vertex> all_Vertices;
		std::vector<unsigned int> all_Indices;
		std::vector<SubMeshInfo> subMeshInfos;

		uint32_t VBO = 0;
		uint32_t EBO = 0;
	};

	struct MeshData3D : public MeshData
	{
	public:
		MeshData3D() = default;

		~MeshData3D();

		SubMeshInfo& AddMesh3DToMeshData(const std::string& name, std::vector<Vertex> v, std::vector<uint32_t> i);
		void AddMeshDataIntoObjectMap(const std::string& name, SubMeshInfo& info);

		SubMeshInfo& getObjectInfo(const std::string& name) { return objectInfo.at(name); };

	private:
		std::unordered_map<std::string, SubMeshInfo> objectInfo;
	};
	
}
