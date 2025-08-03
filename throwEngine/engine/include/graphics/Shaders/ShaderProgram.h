#pragma once
#include <string>
#include <glm/ext.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SHADER {

	class GLShaderProgram
	{
	public:
		GLShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~GLShaderProgram();
		
		bool SetUpShader(const std::string& vertexPath, const std::string& fragmentPath);

		void bind() const noexcept;
		void unbind() const noexcept;
		void cleanUp() const noexcept;

		[[nodiscard]] uint32_t getProgramID() const noexcept;
		[[nodiscard]] bool hasUniform(const std::string& name) const noexcept;

		void setUniform(const std::string& name, const glm::mat4& type) const;
		void setUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;
		void setUniforms(const glm::mat4& mvp) const;

		void setBool(const std::string& name, bool value) const noexcept;
		void setUint(const std::string& name, uint value) const noexcept;
		void setInt(const std::string& name, int value) const noexcept;
		void setFloat(const std::string& name, float value) const noexcept;
		void setVec2(const std::string& name, const glm::vec2& value) const noexcept;
		void setVec2(const std::string& name, float x, float y) const noexcept;
		void setVec3(const std::string& name, const glm::vec3& value) const noexcept;
		void setVec3(const std::string& name, float x, float y, float z) const noexcept;
		void setVec4(const std::string& name, const glm::vec4& value) const noexcept;
		void setVec4(const std::string& name, float x, float y, float z, float w) const noexcept;
		void setMat2(const std::string& name, const glm::mat2& mat) const noexcept;
		void setMat3(const std::string& name, const glm::mat3& mat) const noexcept;
		void setMat4(const std::string& name, const glm::mat4& mat) const noexcept;

	protected:
		void createShader(GLenum shaderType, const char* shaderSourceCode);
		[[nodiscard]] bool createProgram(unsigned int vertex, unsigned int fragment);
		[[nodiscard]] bool checkShaderCompilingErrors(const GLenum shaderType = 0, unsigned int shader = 0);

	private:
		unsigned int m_vertexID, m_fragmentID;
		unsigned int m_programID;

		bool m_isValid;
	};
}


