#include "graphics/Lighting/LightData.h"
#include "graphics/Lighting/Light.h"

#include "core/Logger.h"
#include "graphics/GLTransformations/Transformations.h"


namespace LIGHTING
{
	LightData::LightData(const glm::vec3& lightPos)
		: m_pos(lightPos),
		m_direction(glm::normalize(m_SceneCenter - m_pos)),
		m_SceneCenter(glm::vec3{ 0.0f })
	{
	}

	// calculation direction of the light, based scene center for now!
	void LightData::calculateDirection(const std::shared_ptr<Light>& light)
	{
		const auto& visual = light->getVisual();

		if (!light || !visual) {
			Logger::warn("[LightData::calculateDirection] light object is nullptr or getVisual nullptr check up!");
			return;
		}

		const auto& transform = visual->getTransform();
		if (!transform) {
			Logger::warn("[LightData::calculateDirection] transform object is nullptr!");
			return;
		}

		switch (light->getType()) {
			case LightType::Directional:
				m_direction = glm::normalize(glm::vec3(0.0) - transform->getPosition());
				break;

			case LightType::Spot:
				// use forward direction of the object (assuming -Z is forward)
				m_direction = glm::normalize(transform->getForward());
				break;

			case LightType::Point:
				// No direction needed
				break;

			default:
				Logger::warn("[LightData::calculateDirection] unknown light type!]");
				break;
		}
	}

	void LightData::setPosition(const glm::vec3 &pos) {
		if (m_pos != pos) {
			m_pos = pos;
		}
	}
}
