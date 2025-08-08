#include "graphics/Mesh/MeshData3D.h"
#include <glad/glad.h>


namespace Graphics
{

	void MeshData::AddSubMeshInfo(const SubMeshInfo s)
	{
		this->subMeshInfos.push_back(s);
	}

	SubMeshInfo& MeshData::AddMesh(std::vector<Vertex> v, std::vector<uint32_t> i)
	{
		/* So each subclass keeps its own SubMeshInfo. Only when rendering! */
		/* so we just have to call info.indexOffset*sizeof, info.vertexOffset */ // etc.

		SubMeshInfo info{};

		// set offset
		info.vertexOffset = all_Vertices.size();
		info.indexOffset = all_Indices.size();

		// set count
		info.vertexCount = v.size();
		info.indexCount = i.size();

		// sum vertices into the all_vertices
		all_Vertices.insert( all_Vertices.end(), v.begin(), v.end() );

		// sum indices into the all_indices ( adjusted by vertexOffSet )
		for (const auto idx : i) {
			all_Indices.push_back(idx + info.vertexOffset);
		}

		// push info into the SubMeshInfo struct
		AddSubMeshInfo(info);

		return subMeshInfos.back();
	}

	MeshData3D::~MeshData3D()
	{
		for (auto& info : subMeshInfos) {

			if (info.VAO != 0) {
				glDeleteVertexArrays(1, &info.VAO);
				info.VAO = 0;
			}
		}
	}

	SubMeshInfo& MeshData3D::AddMesh3DToMeshData(const std::string& name, std::vector<Vertex> v, std::vector<uint32_t> i)
	{
		SubMeshInfo& info = MeshData::AddMesh(v, i);
		AddMeshDataIntoObjectMap(name, info);
		return info;
	}

	void MeshData3D::AddMeshDataIntoObjectMap(const std::string& name, SubMeshInfo& info)
	{
		objectInfo[name] = info;
	}
	
}