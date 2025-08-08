#include "graphics/Lighting/LightData.h"
#include "graphics/Lighting/Light.h"

#include "core/Logger.h"
#include "graphics/Transformations/Transformations.h"


namespace LIGHTING
{
	LightData::LightData(const glm::vec3& lightPos)
		: m_pos(lightPos),
		m_direction(glm::normalize(m_SceneCenter - m_pos)),
		m_SceneCenter(glm::vec3{ 0.0f })
	{
	}

	// calculation direction of the light, based scene center for now!
	void LightData::updateAll(const std::shared_ptr<Light>& light)
	{
	}

	void LightData::setPosition(const glm::vec3 &pos) {
		if (m_pos != pos) {
			m_pos = pos;
		}
	}
}
