#include "graphics/Renderer/Renderer.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Shaders/ShaderManager.h"

#include "graphics/Camera/Camera.h"
#include "Scene/Scene.h"
#include "graphics/Textures/Textures.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)


namespace Graphics
{
	Renderer::Renderer(const std::shared_ptr<RenderData>& renderData)
		: m_renderData(renderData)
	{
		DEBUG_PTR(renderData);
	}


	void Renderer::draw(std::shared_ptr<SCENE::Scene>& scene, const glm::mat4& view, const glm::mat4& projection) const
	{
		/* do it some stuff */
		if (!m_renderData) {
			Logger::warn("[Renderer::draw] m_renderData nullptr!");
			return;
		}

		if (!scene) {
			Logger::warn("[Renderer::draw] scene nullptr!");
			return;
		}

		scene->updateInputComponents();

		scene->drawAllObjects(view, projection, m_renderData);
	}
}