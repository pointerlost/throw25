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

	class Light
	{
	public:
		explicit Light(const std::shared_ptr<LightData>& lightData);

		void update(const std::shared_ptr<Light>& light) const;

		void setType(const LightType type) { m_type = type; };
		[[nodiscard]] LightType getType() const { return m_type; };

		[[nodiscard]] std::shared_ptr<LightData> getLightData() const { return m_lightData; };

	private:
		// Light data storage the light properties for sending gpu (with uniforms etc.)
		std::shared_ptr<LightData> m_lightData;

		// Light type, point = 0, directional = 1, spot = 2 (changeable with new properties later maybe.)
		LightType m_type;
	};
}