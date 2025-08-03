#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "nlohmann/json.hpp"

#include "graphics/Shaders/ShaderInterface.h"

namespace SHADER
{
	class GLShaderProgram;

	class ShaderManager
	{
	public:
		ShaderManager() = default;

		bool loadAllShaders();

		bool loadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath, bool isHelper, ShaderType type = ShaderType::BASIC);

		bool createFallbackShader();

		ShaderType getType(const nlohmann::json& shaderConfig);
		std::string getName(const nlohmann::json& shaderConfig) const;

		void addShaderInterface(const std::string& name, const std::shared_ptr<IShader>& shader);
		std::shared_ptr<IShader> getShaderInterface(const std::string& name);
		std::vector<std::shared_ptr<IShader>> getAllShaderInterfaces();

		void setWrapperGLShader(const std::shared_ptr<GLShaderProgram> wrapper) { m_wrapperShader = wrapper; };
		std::shared_ptr<GLShaderProgram>& getWrapperGLShader() { return m_wrapperShader; };

	private:
		std::unordered_map<std::string, std::shared_ptr<IShader>> shaderInterfaces_;
		std::shared_ptr<GLShaderProgram> m_wrapperShader;
	};
}
