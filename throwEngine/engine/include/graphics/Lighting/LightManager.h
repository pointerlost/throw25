#pragma once
#include <vector>
#include <memory>
#include <stack>

#include "graphics/Lighting/Light.h"

static constexpr int MAX_DYNAMIC_LIGHTS = 8;
static constexpr int MAX_STATIONARY_LIGHTS = 24;

namespace LIGHTING
{
	class LightData;
}

namespace SHADER
{
	class GLShaderProgram;
}

namespace LIGHTING
{

	class LightManager
	{
	public:
		LightManager() = default;

		void uploadLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader, LIGHTING::LightMobility mobility) const;

		void uploadStaticLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader)     const;
		void uploadStationaryLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader) const;
		void uploadDynamicLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader)    const;

		[[nodiscard]] uint32_t getActiveLightCount() const { return m_totalLightCount; };

		void addStaticLight(const std::weak_ptr<Light>& light);
		[[nodiscard]] const std::vector<std::weak_ptr<Light>>& getStaticLights() const { return m_staticLightsVec; };
		[[nodiscard]] uint32_t getStaticLightCount() const { return m_staticLightsVec.size(); };

		void addStationaryLight(const std::weak_ptr<Light>& light);
		void setStationaryLightsDirty(bool dirty) { m_stationaryLightsDirty = dirty; };
		[[nodiscard]] bool areStationaryLightsDirty() const { return m_stationaryLightsDirty; };
		[[nodiscard]] const std::array<std::weak_ptr<Light>, MAX_STATIONARY_LIGHTS>& getStationaryLights() const { return m_stationaryLights; };
		[[nodiscard]] uint32_t getStationaryLightCount() const { return m_lastIdxStationary - m_freeStationaryIndices.size(); };

		void addDynamicLight(const std::weak_ptr<Light>& light);
		void setDynamicLightsDirty(bool dirty) { m_dynamicLightsDirty = dirty; };
		[[nodiscard]] bool areDynamicLightsDirty() const { return m_dynamicLightsDirty; };
		[[nodiscard]] const std::array<std::weak_ptr<Light>, MAX_DYNAMIC_LIGHTS>& getDynamicLights() const { return m_dynamicLights; };
		[[nodiscard]] uint32_t getDynamicLightCount() const { return m_lastIdxDynamic - m_freeDynamicIndices.size(); };

		void calculateLightCount();
		void cleanupExpiredLights();

	private:
		uint m_totalLightCount = 0;

		std::vector<std::weak_ptr<Light>> m_staticLightsVec;
		bool m_staticLightsDirty = false;
		std::unordered_map<std::string, std::weak_ptr<Light>> m_staticLightsMap;

		size_t m_lastIdxStationary = 0;
		std::array<std::weak_ptr<Light>, MAX_STATIONARY_LIGHTS> m_stationaryLights;
		std::stack<uint16_t> m_freeStationaryIndices;
		bool m_stationaryLightsDirty = false;

		size_t m_lastIdxDynamic = 0;
		std::array<std::weak_ptr<Light>, MAX_DYNAMIC_LIGHTS> m_dynamicLights;
		std::stack<uint16_t> m_freeDynamicIndices;
		bool m_dynamicLightsDirty = false;
	};
}
