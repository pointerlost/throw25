#include "graphics/Grid/GridSystem.h"

#include "graphics/Grid/GridData.h"

#include "graphics/Grid/GridRenderer.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace GRID
{
	bool GridSystem::initialize(std::shared_ptr<CAMERA::Camera> camera)
	{
		m_gridData = std::make_shared<GridData>();
		if (!m_gridData) {
			Logger::error("[GridSystem] Failed to create grid data");
			return false;
		}

		m_renderer = std::make_unique<GridRenderer>(m_gridData, camera);
		if (!m_renderer) {
			Logger::error("[GridSystem] Failed to create grid renderer");
			return false;
		}

		return true;
	}

	std::shared_ptr<GridData> GridSystem::getData()
	{
		if (!m_gridData) {
			Logger::warn("[GridSystem::getData()] gridData returning nullptr!");
			return nullptr;
		}

		return m_gridData;
	}

	GridRenderer* GridSystem::getRenderer()
	{
		if (!m_renderer) {
			Logger::warn("[GridSystem::getRenderer()] gridRenderer returning nullptr!");
			return nullptr;
		}

		return m_renderer.get();
	}
}