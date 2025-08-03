#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <memory>
#include <glm/gtx/string_cast.hpp>

namespace SCENE { class Scene;	  };

namespace GLgraphics
{
	// Forward declarations
	class RenderData;

	class Renderer
	{
	public:
		Renderer(const std::shared_ptr<RenderData>& renderData);
		Renderer() = default;

		void draw(std::shared_ptr<SCENE::Scene>& scene, const glm::mat4& view, const glm::mat4& projection) const;

	protected:
		std::shared_ptr<RenderData> m_renderData;
	};
}
