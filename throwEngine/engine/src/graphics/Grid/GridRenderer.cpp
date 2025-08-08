#include "graphics/Grid/GridRenderer.h"

#include "graphics/Grid/GridData.h"

#include "graphics/Camera/Camera.h"

#include "graphics/Shaders/ShaderProgram.h"

#include "graphics/Shaders/GridShader.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace GRID
{
	GridRenderer::GridRenderer(std::shared_ptr<GridData> gridData, std::shared_ptr<Graphics::Camera> camera) :
		m_gridData(gridData), m_camera(camera)
	{
		DEBUG_PTR(m_gridData);
		DEBUG_PTR(m_camera);
	}

	void GridRenderer::draw() const
	{
		if (!m_gridShader || !m_camera) return;

		auto shaderProgram = m_gridShader->getGLShaderProgram();
		if (!shaderProgram) return;

		shaderProgram->bind();

		// Set required uniforms
		shaderProgram->setVec3("u_cameraPos", m_camera->getCameraPosition());
		shaderProgram->setFloat("u_fadeStart", 50.0f);
		shaderProgram->setFloat("u_fadeEnd", 100.0f);

		glBindVertexArray(m_gridData->getVAO());
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_gridData->getGridVerticesVec().size()));
		glBindVertexArray(0);
	}

	bool GridRenderer::setGridShaderInterface(std::shared_ptr<SHADER::IShader> gridShader)
	{
		if (!gridShader) {
			Logger::warn("[GridRenderer::setGridShaderInterface] argument returning nullptr you can't set that pointer!");
			return false;
		}
		
		m_gridShader = gridShader;

		return true;
	}

	std::shared_ptr<SHADER::IShader> GridRenderer::getGridShader()
	{
		if (!m_gridShader) {
			Logger::warn("[ GridRenderer::getGridShader() ] returning nullptr!");
			return nullptr;
		}
		return m_gridShader;
	}
}