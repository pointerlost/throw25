#include "graphics/Lighting/Light.h"

#include "core/Logger.h"
#include "Scene/SceneObject.h"
#include "graphics/Transformations/Transformations.h"

namespace LIGHTING
{
	Light::Light(const std::shared_ptr<LightData>& lightData)
		: m_lightData(lightData), m_type(LightType::Directional) {
		// Setup context
		// set light position of the visual object like a light source!
		// m_lightData->setPosition(m_SceneObject->getTransform()->getPosition());
	}

	void Light::update(const std::shared_ptr<Light>& light) const
	{
		if (!light) {
			Logger::warn("[Light::update]: light nullptr");
			return;
		}
		// I'll create an object-based direction, we are use scene center for now (later)
		m_lightData->updateAll(light);
	}
}
