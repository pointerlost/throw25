#pragma once
#include <memory>
#include <graphics/Lighting/LightData.h>

#include "Scene/SceneObject.h"

namespace LIGHTING
{
	enum class LightType
	{
		Point		= 0,
		Directional = 1,
		Spot		= 2
	};

	enum struct LightMobility {
		Static,
		Stationary,
		Dynamic,
		NonType // used for scene object classification to determine if lighting data should be uploaded
	};

	class Light
	{
	public:
		explicit Light(const std::shared_ptr<LightData>& lightData);

		void update(const std::shared_ptr<Light>& light) const;

		void setLightType(const LightType type) { m_type = type; };
		[[nodiscard]] LightType getType() const { return m_type; };

		void setVisualObject(const std::shared_ptr<SCENE::SceneObject>& lightObject) { m_visual = lightObject; };
		[[nodiscard]] std::shared_ptr<SCENE::SceneObject> getVisual() const;

		[[nodiscard]] std::shared_ptr<LightData> getLightData() const { return m_lightData; };

		void setMobility(const LightMobility usage) { m_usage = usage; };
		[[nodiscard]] LightMobility getMobility() const { return m_usage; };

	private:
		// Light data storage the light properties for sending gpu (with uniforms etc.)
		std::shared_ptr<LightData> m_lightData;

		// this pointer using to representation of light on the scene
		std::shared_ptr<SCENE::SceneObject> m_visual;

		// Light type, point = 0, directional = 1, spot = 2 (changeable with new properties later maybe.)
		LightType m_type;

		LightMobility m_usage;
	};
}