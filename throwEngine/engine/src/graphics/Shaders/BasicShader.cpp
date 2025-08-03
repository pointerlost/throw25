#include <graphics/Shaders/BasicShader.h>

#include <graphics/Shaders/ShaderProgram.h>

#include <graphics/Material/MaterialLib.h>

#include "graphics/Lighting/Light.h"

#include "graphics/Lighting/LightManager.h"

#include "Scene/SceneObject.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace SHADER
{
    BasicShader::BasicShader(const std::shared_ptr<GLShaderProgram>& glShader)
        : m_glProgram(glShader)
    {
        Logger::info("BasicShader created as fallback");
    }

    std::shared_ptr<GLShaderProgram> BasicShader::getGLShaderProgram() const
    {
        return m_glProgram;
    }

    void BasicShader::bind()
    {
        m_glProgram ? m_glProgram->bind() : Logger::warn("BasicShader program is can't binding!");
    }

    void BasicShader::setLights(const std::vector<std::shared_ptr<LIGHTING::Light>>& lights)
    {
        if (!m_glProgram) {
            Logger::warn("BasicShader does not have a GLProgram!");
            return;
        }

        constexpr int maxLights = 5; // match MaxLightSize in GLSL
        const int count = static_cast<int>(std::min(lights.size(), static_cast<size_t>(maxLights)));

        for (size_t i = 0; i < count; ++i) {
            const auto& light = lights[i];
            std::string prefix = "lights[" + std::to_string(i) + "]";

            const auto lightData = light->getLightData();

            // add later | ambient, diffuse, specular use getter methods
            m_glProgram->setVec3(prefix + ".position",     lightData->getPosition());
            m_glProgram->setVec3(prefix + ".direction",    lightData->getDirection());
            m_glProgram->setVec3(prefix + ".diffuse",      lightData->getDiffuse());
            m_glProgram->setVec3(prefix + ".specular",      lightData->getSpecular());
            m_glProgram->setFloat(prefix + ".constant",    lightData->getConstant());
            m_glProgram->setFloat(prefix + ".linear",      lightData->getLinear());
            m_glProgram->setFloat(prefix + ".quadratic",   lightData->getQuadratic());
            m_glProgram->setFloat(prefix + ".cutOff",      lightData->getCutOff());
            m_glProgram->setFloat(prefix + ".outerCutOff", lightData->getOuterCutOff());
            m_glProgram->setInt(prefix + ".type", static_cast<int>(light->getType()));
        }

        m_glProgram->setInt("activeLightCount", count);
    }

    void BasicShader::setMaterial(const std::shared_ptr<MATERIAL::Material>& mat)
    {
        if (!m_glProgram || !mat) return;

        m_glProgram->setVec3("material.ambient",    mat->m_ambient);
        m_glProgram->setVec3("material.diffuse",    mat->m_diffuse);
        m_glProgram->setVec3("material.specular",   mat->m_specular);
        m_glProgram->setFloat("material.shininess", mat->m_shininess);

        // Diffuse texture
        if (mat->m_diffuseTexture && mat->m_diffuseTexture->glID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mat->m_diffuseTexture->glID);
            m_glProgram->setInt("material_diffuseTex", 0);
            m_glProgram->setInt("hasDiffuseTex", 1);
        } else {
            m_glProgram->setInt("hasDiffuseTex", 0);
        }

        // Specular texture
        if (mat->m_specularTexture && mat->m_specularTexture->glID != 0) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mat->m_specularTexture->glID);
            m_glProgram->setInt("material_specularTex", 1);
            m_glProgram->setInt("hasSpecularTex", 1);
        } else {
            m_glProgram->setInt("hasSpecularTex", 0);
        }

    }

    void BasicShader::setMatrices(const glm::mat4& model, const glm::mat4& view,
        const glm::mat4& projection, const glm::vec3& cameraPos)
    {
        if (!m_glProgram) return;

        m_glProgram->setUniform("model", model);
        m_glProgram->setUniform("view", view);
        m_glProgram->setUniform("projection", projection);
        m_glProgram->setVec3("viewPos", cameraPos);
    }

 //    void BasicShader::setShaderInterface(const std::shared_ptr<SCENE::SceneObject>& lightObject)
 //    {
 //        if (!lightObject) {
 //            Logger::warn("[BasicShader::setShaderInterface] lightObject is nullptr!");
 //            return;
 //        }
 //        m_shaderInterface = lightObject;
 //        DEBUG_PTR(m_shaderInterface);
	// }
 //
 //    std::shared_ptr<SCENE::SceneObject> BasicShader::getShaderInterface() const
 //    {
 //        if (!m_shaderInterface) {
 //            Logger::warn("[BasicShader::getShaderInterface] m_glProgram is nullptr!");
 //            return nullptr;
 //        }
	// 	return m_shaderInterface;
 //    }

}