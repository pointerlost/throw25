#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <stack>

namespace LIGHTING
{
	class LightData;
	class Light;
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

		void setLightUniforms(const std::shared_ptr<SHADER::GLShaderProgram>& shader,
			size_t index, const std::shared_ptr<Light>& light);

		void uploadLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader);

		[[nodiscard]] uint32_t getActiveLightCount() const { return m_lightMap.size(); };

		std::vector<std::shared_ptr<Light>> getLights() const { return m_lights; }

		bool checkLightExists(const std::string& name) const;

		void addLight(const std::shared_ptr<Light>& light);
		void removeLight(const std::string& name);
		[[nodiscard]] std::shared_ptr<Light> getLight(const std::string& name) const;

	private:
		std::vector<std::shared_ptr<Light>> m_lights;

		std::stack<size_t> m_NullIndexStackOfLights;

		std::unordered_map<std::string, uint32_t> getFreeIndexWithName;

		// get light with the name of scene object
		std::unordered_map<std::string, std::shared_ptr<Light>> m_lightMap;
	};
}
