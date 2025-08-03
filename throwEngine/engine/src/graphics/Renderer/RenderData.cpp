#include "graphics/Renderer/RenderData.h"

#include "graphics/Shaders/ShaderProgram.h"

#include "graphics/Shaders/ShaderManager.h"

#include "graphics/Camera/Camera.h"

#include <graphics/Textures/Textures.h>

#include "graphics/Material/MaterialLib.h"

#include "core/Debug.h"
#include "graphics/Shaders/BasicShader.h"
#include "core/Logger.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace GLgraphics
{
	RenderData::RenderData(std::shared_ptr<SHADER::ShaderManager> shaderManager, const std::shared_ptr<CAMERA::Camera>& camera,
		const std::shared_ptr<TEXTURE::TextureManager>& textureManager, const std::shared_ptr<MATERIAL::MaterialLibrary>& materialLib)
		: m_shaderManager(shaderManager), m_textureManager(textureManager),
		m_material(materialLib), m_camera(camera), g_ambientLight(glm::vec3(0.1, 0.1, 0.1))
	{
		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);

		DEBUG_PTR(m_shaderManager);
		DEBUG_PTR(m_camera);
		DEBUG_PTR(m_textureManager);
		DEBUG_PTR(m_material);
	}
	
	// VIEW (CAMERA) STUFF
	void RenderData::setViewMatrix(const glm::mat4 view)
	{
		this->view = view;
	}

	// PROJECTION STUFF
	void RenderData::setProjectionMatrix(const glm::mat4 projection)
	{
		this->projection = projection;
	}

	std::shared_ptr<SHADER::IShader> RenderData::getShaderInterface(const std::string& name)
	{
		return m_shaderManager->getShaderInterface(name);
	}

	std::shared_ptr<SHADER::GLShaderProgram>& RenderData::getWrapperGLShader()
	{
		return m_shaderManager->getWrapperGLShader();
	}

	std::shared_ptr<MATERIAL::MaterialLibrary> RenderData::getMaterialLib() {
		if (!m_material) {
			Logger::warn("[RenderData::getMaterialLib] m_material is nullptr!\n");
			return {};
		}
		return m_material;
	}

	// UPDATE MVP
	void RenderData::update()
	{
		view = m_camera->getViewMatrix();
		projection = m_camera->getProjectionMatrix();
	}

}