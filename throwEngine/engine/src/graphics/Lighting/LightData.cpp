#include "graphics/Lighting/LightData.h"



namespace LIGHTING
{
	LightData::LightData(const glm::vec3& lightPos)
		: m_pos(lightPos),
		m_direction(glm::normalize(m_SceneCenter - m_pos)),
		m_SceneCenter(glm::vec3{ 0.0f })
	{
	}

	// when sun or an other light objects of changing position, so we have to calculate direction again!
	void LightData::calculateDirection(const glm::vec3& sceneCenter)
	{
		m_direction = glm::normalize(m_SceneCenter - m_pos);
	}

}
