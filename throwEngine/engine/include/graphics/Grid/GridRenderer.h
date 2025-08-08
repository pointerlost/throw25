#pragma once
#include <iostream>
#include <memory>

namespace Graphics { class Camera;  };
namespace SHADER
{
	class IShader;
	class GridShader;
};

namespace GRID
{
	// Forward Declaration
	class GridData;

	class GridRenderer
	{
	public:
		GridRenderer(std::shared_ptr<GridData> gridData, std::shared_ptr<Graphics::Camera> camera);
		~GridRenderer() = default;

		void draw() const;

		std::shared_ptr<GridData>& getGridData() { return m_gridData; };

		bool setGridShaderInterface(std::shared_ptr<SHADER::IShader> gridShader);
		std::shared_ptr<SHADER::IShader> getGridShader();

	private:
		std::shared_ptr<GridData> m_gridData;
		std::shared_ptr<SHADER::IShader> m_gridShader;
		std::shared_ptr<Graphics::Camera> m_camera;
	};


}