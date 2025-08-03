#include "graphics/Lighting/Light.h"

#include "core/Logger.h"
#include "Scene/SceneObject.h"
#include "graphics/GLTransformations/Transformations.h"

namespace LIGHTING
{
	Light::Light(const std::shared_ptr<LightData>& lightData)
		: m_lightData(lightData), m_type(LightType::Directional), m_usage(LightMobility::Static) {
		// Setup context
		// set light position of the visual object like a light source!
		// m_lightData->setPosition(m_SceneObject->getTransform()->getPosition());
	}

	void Light::update(const std::shared_ptr<SCENE::SceneObject>& lightObject) const
	{
		m_lightData->setPosition(lightObject->getTransform()->getPosition());
		m_lightData->calculateDirection(lightObject->getTransform()->getPosition());
	}

	std::weak_ptr<SCENE::SceneObject> Light::getVisual() const {
		if (m_visual.expired()) {
			Logger::warn("[Light] Visual object has expired!\n");
			return {};
		}
		return m_visual;
	}
}
