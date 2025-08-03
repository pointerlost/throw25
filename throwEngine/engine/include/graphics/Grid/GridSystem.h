#pragma once
#include <iostream>
#include <memory>
#include "graphics/Grid/GridRenderer.h"

namespace CAMERA { class Camera;   };

namespace GRID
{
	class GridData;
	class GridRenderer;

	class GridSystem
	{
	public:
		GridSystem() = default;

		bool initialize(std::shared_ptr<CAMERA::Camera> camera);

		std::shared_ptr<GridData> getData();
		GridRenderer* getRenderer();

	private:
		std::shared_ptr<GridData> m_gridData;
		std::unique_ptr<GridRenderer> m_renderer;
	};
}