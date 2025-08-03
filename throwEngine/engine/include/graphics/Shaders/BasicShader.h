#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "graphics/Shaders/ShaderInterface.h"

namespace LIGHTING { class  Light;	     };
namespace MATERIAL { struct Material;  	 };
namespace SCENE    { class  SceneObject; };

namespace SHADER
{
	class GLShaderProgram;

	class BasicShader : public IShader
	{
	public:
		explicit BasicShader(const std::shared_ptr<GLShaderProgram>& glShader);

		[[nodiscard]] ShaderType getType() const override { return m_type; };
		[[nodiscard]] std::shared_ptr<GLShaderProgram> getGLShaderProgram() const override;
		void bind() override;
		void setLights(const std::vector<std::shared_ptr<LIGHTING::Light>>& lights) override;
		void setMaterial(const std::shared_ptr<MATERIAL::Material>& mat) override;
		void setMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) override;

		// void setShaderInterface(const std::shared_ptr<SCENE::SceneObject>& lightObject);
		// [[nodiscard]] std::shared_ptr<SCENE::SceneObject> getShaderInterface() const override;

	private:
		std::shared_ptr<SCENE::SceneObject> m_shaderInterface;

		std::shared_ptr<GLShaderProgram> m_glProgram;
		glm::vec3 m_fallbackColor = glm::vec3(1.0, 0.0, 1.0f); // purple = error color

		ShaderType m_type = ShaderType::BASIC;
	};
}
