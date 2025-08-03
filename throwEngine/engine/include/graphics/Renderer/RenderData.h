#pragma once
#include <memory>
#include <glm/ext.hpp>

#include "graphics/Shaders/ShaderManager.h"
#include "graphics/Grid/GridRenderer.h"

// Forward Declarations;
namespace CAMERA   { class Camera;			};
namespace TEXTURE  { class TextureManager;	};
namespace MATERIAL { class MaterialLibrary; };
namespace SHADER   { class GLShaderProgram; };
namespace LIGHTING
{
	class Light;
	class LightManager;
};

namespace GLgraphics
{
	
	class RenderData
	{
	public:
		RenderData(std::shared_ptr<SHADER::ShaderManager> shaderManager,
			const std::shared_ptr<CAMERA::Camera>& camera,
			const std::shared_ptr<TEXTURE::TextureManager>& textureManager,
			const std::shared_ptr<MATERIAL::MaterialLibrary>& materialLib);

		~RenderData() = default;

		void setViewMatrix(const glm::mat4 view);
		void setProjectionMatrix(const glm::mat4 projection);

		void setLightManager(const std::shared_ptr<LIGHTING::LightManager>& lightManager) { m_lightManager = lightManager; };
		[[nodiscard]] std::shared_ptr<LIGHTING::LightManager> getLightManager() { return m_lightManager; };

		void setGridRenderer(GRID::GridRenderer* gridRenderer) { m_gridRenderer = gridRenderer; };
		[[nodiscard]] GRID::GridRenderer* getGridRenderer() { return m_gridRenderer; };
		
		[[nodiscard]] glm::mat4 getViewMatrix() const { return view; };
		[[nodiscard]] glm::mat4 getProjectionMatrix() const { return projection; };

		[[nodiscard]] std::shared_ptr<SHADER::ShaderManager> getShaderManager() const { return m_shaderManager; };
		[[nodiscard]] std::shared_ptr<SHADER::IShader> getShaderInterface(const std::string& name);
		[[nodiscard]] std::shared_ptr<SHADER::GLShaderProgram>& getWrapperGLShader();
		//std::shared_ptr<SHADER::Shader> getWrappedShader(const std::string& name);
		[[nodiscard]] const std::shared_ptr<CAMERA::Camera> getCamera() const { return m_camera; };
		[[nodiscard]] const std::shared_ptr<TEXTURE::TextureManager> getTextureManager() const { return m_textureManager; };
		[[nodiscard]] std::shared_ptr<MATERIAL::MaterialLibrary> getMaterialLib();

		[[nodiscard]] const glm::vec3& getGlobalAmbient() const { return g_ambientLight; };

		void update();

	protected:
		
		std::shared_ptr<LIGHTING::LightManager> m_lightManager;
		std::shared_ptr<SHADER::ShaderManager> m_shaderManager;
		std::shared_ptr<TEXTURE::TextureManager> m_textureManager;
		std::shared_ptr<MATERIAL::MaterialLibrary> m_material;
		std::shared_ptr<CAMERA::Camera> m_camera;
		GRID::GridRenderer* m_gridRenderer = nullptr;

	private:
		// Copy MVP matrices into RenderData
		// Because if we don't copy, we have to give reference all of the time, so this is cause complexity
		glm::mat4 view;
		glm::mat4 projection;

		glm::vec3 g_ambientLight;
	};
}
