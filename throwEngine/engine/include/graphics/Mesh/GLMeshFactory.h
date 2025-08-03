#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <unordered_map>
#include <functional>


namespace GLgraphics
{
	struct Vertex;

	class MeshFactory
	{
	public:
		MeshFactory();

		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createTriangle();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createSquare();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createCube();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createCircle();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createSphere();

		std::pair<std::vector<Vertex>, std::vector<uint32_t>> createMeshObject(const std::string &name);

		// add cube, sphere, etc.
	private:
		std::unordered_map<std::string, std::function<std::pair<std::vector<Vertex>, std::vector<uint32_t>>()>> meshObjects;
		
		void addObjectsIntoMap();
	};
}
