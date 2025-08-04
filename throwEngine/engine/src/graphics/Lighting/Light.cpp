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

	void Light::update(const std::shared_ptr<Light>& light) const
	{
		if (!light) {
			Logger::warn("[Light::update]: light is null");
			return;
		}
		const auto& visual = light->getVisual();
		if (!visual) {
			Logger::warn("[Light::update] visual object expired!");
			return;
		}
		m_lightData->setPosition(visual->getTransform()->getPosition());
		// I'll create an object-based direction, we are use scene center for now (later)
		m_lightData->calculateDirection(light);
	}

	std::shared_ptr<SCENE::SceneObject> Light::getVisual() const {
		if (!m_visual) {
			Logger::warn("[Light] Visual object nullptr!\n");
			return {};
		}
		return m_visual;
	}
}
