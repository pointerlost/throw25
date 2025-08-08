#include "graphics/Mesh/MeshFactory.h"
#include <graphics/Mesh/MeshData3D.h>

namespace Graphics
{
	MeshFactory::MeshFactory()
	{
		addObjectsIntoMap();
	};

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::createTriangle()
	{
		std::vector<Vertex> vertices =
		{	// Vertices						   // Normal					// Color
			{ glm::vec3(-0.5f, 0.5f, 0.0f),    glm::vec3{0.0, 0.0, 1.0},    glm::vec3{0.5, 0.5, 1.0}, glm::vec2{0.0, 1.0},    /* top left   */  },
			{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3{0.0, 0.0, 1.0},    glm::vec3{0.5, 0.5, 1.0}, glm::vec2{0.0, 0.0},    /* down left  */  },
			{ glm::vec3(0.5f, 0.5f, 0.5f),     glm::vec3{0.0, 0.0, 1.0},    glm::vec3{0.5, 0.5, 1.0}, glm::vec2{1.0, 1.0},    /* top right  */  },
			{ glm::vec3(0.5f, -0.5f, 0.5f),    glm::vec3{0.0, 0.0, 1.0},    glm::vec3{0.5, 0.5, 1.0}, glm::vec2{1.0, 0.0},    /* down right */  },
			{ glm::vec3(0.0f, 0.5f, 0.0f),     glm::vec3{0.0, 1.0, 0.0},    glm::vec3{0.5, 0.5, 1.0}, glm::vec2{0.5, 1.0}    /* top center */  },
		};

		std::vector<uint32_t> indices =
		{
			2, 3, 4,
		};

		return std::make_pair(vertices, indices);
	}


	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::createSquare()
	{
		std::vector<Vertex> vertices =
		{	// Vertices						   Normal					  Color
			{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0, 0.0, -1.0),  glm::vec3(1.0, 1.0, 0.0) /* top left   */  },
			{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0, 0.0, -1.0),  glm::vec3(1.0, 1.0, 0.0) /* down left  */  },
			{ glm::vec3( 0.5f,  0.5f, 0.5f),   glm::vec3(0.0, 0.0, -1.0),  glm::vec3(1.0, 1.0, 0.0) /* top right  */  },
			{ glm::vec3( 0.5f, -0.5f, 0.5f),   glm::vec3(0.0, 0.0, -1.0),  glm::vec3(1.0, 1.0, 0.0) /* down right */  }
		};

		std::vector<uint32_t> indices =
		{
			0, 1, 2,
			1, 2, 3
		};

		return std::make_pair(vertices, indices);
	}


	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::createCube()
	{
		std::vector<Vertex> vertices =
		{	// Vertices						                       Normal						  Color						     TexCoords
			
			// Front face (+Z)
			{ glm::vec3(-0.5f, -0.5f, 0.5f),   glm::vec3 {0.0,  0.0, 1.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 0.0f}  },
			{ glm::vec3(0.5f,  -0.5f, 0.5f),   glm::vec3 {0.0,  0.0, 1.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 0.0f}  },
			{ glm::vec3(0.5f,   0.5f, 0.5f),   glm::vec3 {0.0,  0.0, 1.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 1.0f}  },
			{ glm::vec3(-0.5f,  0.5f, 0.5f),   glm::vec3 {0.0,  0.0, 1.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 1.0f}  },

			// Back face  (-Z)
			{ glm::vec3(0.5f,  -0.5f, -0.5f),  glm::vec3 {0.0,  0.0, -1.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 0.0f}  },
			{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3 {0.0,  0.0, -1.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 0.0f}  },
			{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3 {0.0,  0.0, -1.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 1.0f}  },
			{ glm::vec3(0.5f,   0.5f, -0.5f),  glm::vec3 {0.0,  0.0, -1.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 1.0f}  },

			// Left face  (-X)
			{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3 {-1.0,  0.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 0.0f}  },
			{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3 {-1.0,  0.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 0.0f}  },
			{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3 {-1.0,  0.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 1.0f}  },
			{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3 {-1.0,  0.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 1.0f}  },

			// Right face (+X)
			{ glm::vec3(0.5f, -0.5f,  0.5f),   glm::vec3 {1.0,  0.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 0.0f}  },
			{ glm::vec3(0.5f, -0.5f, -0.5f),   glm::vec3 {1.0,  0.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 0.0f}  },
			{ glm::vec3(0.5f,  0.5f, -0.5f),   glm::vec3 {1.0,  0.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 1.0f}  },
			{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3 {1.0,  0.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 1.0f}  },

			// Top face (+Y)
			{ glm::vec3(-0.5f, 0.5f,  0.5f),   glm::vec3 {0.0,  1.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 0.0f}  },
			{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3 {0.0,  1.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 0.0f}  },
			{ glm::vec3(0.5f,  0.5f, -0.5f),   glm::vec3 {0.0,  1.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 1.0f}  },
			{ glm::vec3(-0.5f, 0.5f, -0.5f),   glm::vec3 {0.0,  1.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 1.0f}  },

			// Bottom face (-Y)
			{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3 {0.0, -1.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 0.0f}  },
			{ glm::vec3(0.5f,  -0.5f, -0.5f),  glm::vec3 {0.0, -1.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 0.0f}  },
			{ glm::vec3(0.5f,  -0.5f,  0.5f),  glm::vec3 {0.0, -1.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {1.0f, 1.0f}  },
			{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3 {0.0, -1.0, 0.0},   glm::vec3 {0.6f, 0.0f, 0.0f},  {0.0f, 1.0f}  },
		};

		std::vector<uint32_t> indices =
		{
			// Front face
			0, 1, 2,      2, 3, 0,

			// Back face
			4, 5, 6,      6, 7, 4,

			// Left face
			8, 9, 10,     10, 11, 8,

			// Right face
			12, 13, 14,   14, 15, 12,

			// Top face
			16, 17, 18,   18, 19, 16,

			// Bottom face
			20, 21, 22,   22, 23, 20,
		};

		return std::make_pair(vertices, indices);
	}

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::createCircle()
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		int segmentCount = 30;
		float radius = 1.0f;

		glm::vec3 centerColor = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec2 centerTexCoord = glm::vec2(0.5f, 0.5f);
		glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);

		vertices.push_back({ glm::vec3(0.0f,0.0f,0.0f), normal, centerColor, centerTexCoord }); // center vertex must be first

		for (int i = 0; i < segmentCount; i++) {

			float angle = 2.0f * M_PI * i / segmentCount;
			float x = radius * cos(angle);
			float y = radius * sin(angle);
			float z = 0.0f;

			glm::vec3 position = glm::vec3(x, y, z);
			glm::vec3 color = glm::vec3((cos(angle) + 1.0f) * 0.5f, (sin(angle) + 1.0f) * 0.5f, 1.0f); // color changing
			glm::vec2 texCoord = glm::vec2((x + 1.0f) * 0.5f, (y + 1.0f) * 0.5f); // for texture normalizing

			vertices.push_back({ position, normal, color, texCoord });
		}

		for (int i = 1; i <= segmentCount; i++) {
			indices.push_back(0); // center vertex
			indices.push_back(i);
			if (i == segmentCount)
				indices.push_back(1); // wrap around
			else
				indices.push_back(i + 1);
		}

		return std::make_pair(vertices, indices);
	}


	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::createSphere()
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		const unsigned int X_SEGMENTS = 32;
		const unsigned int Y_SEGMENTS = 32;
		const float PI = 3.14159265359f;

		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				glm::vec3 pos = glm::vec3(xPos * 1.1f, yPos * 1.1f, zPos * 1.1f);
				glm::vec3 normal = glm::normalize(glm::vec3(xPos, yPos, zPos));

				vertices.push_back({
					// where the multiplied numbers are the radius of the sphere
					pos,	// pos
					normal, // normal
					glm::vec3(1.0f, 0.7f, 0.1f), // COLOR
					glm::vec2(1.0f, 0.9f)		 // texCoord
				});
			}
		}

		for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
			for (unsigned int x = 0; x < X_SEGMENTS; ++x) {
				uint32_t i0 = y * (X_SEGMENTS + 1) + x;
				uint32_t i1 = i0 + X_SEGMENTS + 1;

				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i0 + 1);

				indices.push_back(i0 + 1);
				indices.push_back(i1);
				indices.push_back(i1 + 1);
			}
		}

		return std::make_pair(vertices, indices);

	};


	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::createMeshObject(const std::string &name)
	{
		auto it = meshObjects.find(name);

		if (it != meshObjects.end()) {

			return it->second();
		}
		else {
			std::cout << name << " Object is not found!" << "\n";
		}
		return{};
	}

	void MeshFactory::addObjectsIntoMap()
	{
		meshObjects["triangle"] = createTriangle;
		meshObjects["square"]   = createSquare;
		meshObjects["cube"]     = createCube;
		meshObjects["circle"]   = createCircle;
		meshObjects["sphere"]	= createSphere;
	}
}