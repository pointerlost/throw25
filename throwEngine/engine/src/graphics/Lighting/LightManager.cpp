#include "graphics/Lighting/LightManager.h"

#include "graphics/Shaders/ShaderProgram.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Lighting/Light.h"

#include <Scene/SceneObject.h>

#include "core/Logger.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace LIGHTING
{
    void LightManager::uploadLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader,
        LIGHTING::LightMobility mobility) const
    {

        switch (mobility) {
            case LIGHTING::LightMobility::Static:
                uploadStaticLights(shader);
                break;
            case LIGHTING::LightMobility::Stationary:
                uploadStationaryLights(shader);
                break;
            case LIGHTING::LightMobility::Dynamic:
                uploadDynamicLights(shader);
                break;
            case LIGHTING::LightMobility::NonType:
                return;

            default:
                // set default lighting values for missing light information to avoid error (BACK LATER!!)
                Logger::warn("Shader doesn't have expected light uniforms");
                break;
        }
    }

    void LightManager::uploadStaticLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader) const
    {
        if (!shader) {
            Logger::error("Shader is null in LightManager::uploadLights");
            return;
        }

        shader->bind();

        for (size_t i = 0; i < m_staticLightsVec.size(); ++i) {
            if (const auto& light = m_staticLightsVec[i].lock())
            {
                std::string prefix = "lights[" + std::to_string(i) + "]";

                const auto lightData = light->getLightData();

                // Position and direction
                shader->hasUniform(prefix + ".position")
                    ? shader->setVec3(prefix + ".position", lightData->getPosition())
                    : Logger::warn("[LightManager::uploadLights] shader has no position uniform!");

                shader->hasUniform(prefix + ".direction")
                    ? shader->setVec3(prefix + ".direction", lightData->getDirection())
                    : Logger::warn("[LightManager::uploadLights] shader has no direction uniform!");

                // Colors
                shader->hasUniform(prefix + ".diffuse")
                    ? shader->setVec3(prefix + ".diffuse", lightData->getDiffuse())
                    : Logger::warn("[LightManager::uploadLights] shader has no diffuse uniform!");

                shader->hasUniform(prefix + ".specular")
                    ? shader->setVec3(prefix + ".specular", lightData->getSpecular())
                    : Logger::warn("[LightManager::uploadLights] shader has no specular uniform!");

                // Attenuation
                shader->hasUniform(prefix + ".constant")
                    ? shader->setFloat(prefix + ".constant", lightData->getConstant())
                    : Logger::warn("[LightManager::uploadLights] shader has no constant uniform!");

                shader->hasUniform(prefix + ".linear")
                    ? shader->setFloat(prefix + ".linear", lightData->getLinear())
                    : Logger::warn("[LightManager::uploadLights] shader has no linear uniform!");

                shader->hasUniform(prefix + ".quadratic")
                    ? shader->setFloat(prefix + ".quadratic", lightData->getQuadratic())
                    : Logger::warn("[LightManager::uploadLights] shader has no quadratic uniform!");

                // Spotlight
                shader->hasUniform(prefix + ".cutOff")
                    ? shader->setFloat(prefix + ".cutOff", lightData->getCutOff())
                    : Logger::warn("[LightManager::uploadLights] shader has no cutOff uniform!");

                shader->hasUniform(prefix + ".outerCutOff")
                    ? shader->setFloat(prefix + ".outerCutOff", lightData->getOuterCutOff())
                    : Logger::warn("[LightManager::uploadLights] shader has no outerCutOff uniform!");

                shader->hasUniform(prefix + ".type")
                    ? shader->setInt(prefix + ".type", static_cast<int>(light->getType()))
                    : Logger::warn("[LightManager::uploadLights] shader has no type uniform!");
            }
            if (shader->hasUniform("activeLightCount"))
                shader->setUint("activeLightCount", m_totalLightCount);
            else
                Logger::warn("[LightManager::uploadLights] shader has no activeLightCount uniform!");
        }
    }

    void LightManager::uploadStationaryLights(const std::shared_ptr<SHADER::GLShaderProgram> &shader) const
    {
        if (!shader) {
            Logger::error("Shader is null in LightManager::uploadLights");
            return;
        }

        shader->bind();

        const size_t count = getStationaryLightCount();
        if (count <= 0) return;

        for (size_t i = 0; i < count; ++i) {
            if (const auto& light = m_stationaryLights[i].lock())
            {
                std::string prefix = "lights[" + std::to_string(i) + "]";

                const auto lightData = light->getLightData();

                // Position and direction
                shader->hasUniform(prefix + ".position")
                    ? shader->setVec3(prefix + ".position", lightData->getPosition())
                    : Logger::warn("[LightManager::uploadLights] shader has no position uniform!");

                shader->hasUniform(prefix + ".direction")
                    ? shader->setVec3(prefix + ".direction", lightData->getDirection())
                    : Logger::warn("[LightManager::uploadLights] shader has no direction uniform!");

                // Colors
                shader->hasUniform(prefix + ".diffuse")
                    ? shader->setVec3(prefix + ".diffuse", lightData->getDiffuse())
                    : Logger::warn("[LightManager::uploadLights] shader has no diffuse uniform!");

                shader->hasUniform(prefix + ".specular")
                    ? shader->setVec3(prefix + ".specular", lightData->getSpecular())
                    : Logger::warn("[LightManager::uploadLights] shader has no specular uniform!");

                // Attenuation
                shader->hasUniform(prefix + ".constant")
                    ? shader->setFloat(prefix + ".constant", lightData->getConstant())
                    : Logger::warn("[LightManager::uploadLights] shader has no constant uniform!");

                shader->hasUniform(prefix + ".linear")
                    ? shader->setFloat(prefix + ".linear", lightData->getLinear())
                    : Logger::warn("[LightManager::uploadLights] shader has no linear uniform!");

                shader->hasUniform(prefix + ".quadratic")
                    ? shader->setFloat(prefix + ".quadratic", lightData->getQuadratic())
                    : Logger::warn("[LightManager::uploadLights] shader has no quadratic uniform!");

                // Spotlight
                shader->hasUniform(prefix + ".cutOff")
                    ? shader->setFloat(prefix + ".cutOff", lightData->getCutOff())
                    : Logger::warn("[LightManager::uploadLights] shader has no cutOff uniform!");

                shader->hasUniform(prefix + ".outerCutOff")
                    ? shader->setFloat(prefix + ".outerCutOff", lightData->getOuterCutOff())
                    : Logger::warn("[LightManager::uploadLights] shader has no outerCutOff uniform!");

                shader->hasUniform(prefix + ".type")
                    ? shader->setInt(prefix + ".type", static_cast<int>(light->getType()))
                    : Logger::warn("[LightManager::uploadLights] shader has no type uniform!");
            }
            if (shader->hasUniform("activeLightCount"))
                shader->setUint("activeLightCount", m_totalLightCount);
            else
                Logger::warn("[LightManager::uploadLights] shader has no activeLightCount uniform!");
        }
    }

    void LightManager::uploadDynamicLights(const std::shared_ptr<SHADER::GLShaderProgram> &shader) const
    {
        if (!shader) {
            Logger::error("Shader is null in LightManager::uploadLights");
            return;
        }

        shader->bind();

        const size_t count = getDynamicLightCount();
        if (count <= 0) return;

        for (size_t i = 0; i < count; ++i) {
            if (const auto& light = m_dynamicLights[i].lock())
            {
                std::string prefix = "lights[" + std::to_string(i) + "]";

                const auto lightData = light->getLightData();

                // Position and direction
                shader->hasUniform(prefix + ".position")
                    ? shader->setVec3(prefix + ".position", lightData->getPosition())
                    : Logger::warn("[LightManager::uploadLights] shader has no position uniform!");

                shader->hasUniform(prefix + ".direction")
                    ? shader->setVec3(prefix + ".direction", lightData->getDirection())
                    : Logger::warn("[LightManager::uploadLights] shader has no direction uniform!");

                // Colors
                shader->hasUniform(prefix + ".diffuse")
                    ? shader->setVec3(prefix + ".diffuse", lightData->getDiffuse())
                    : Logger::warn("[LightManager::uploadLights] shader has no diffuse uniform!");

                shader->hasUniform(prefix + ".specular")
                    ? shader->setVec3(prefix + ".specular", lightData->getSpecular())
                    : Logger::warn("[LightManager::uploadLights] shader has no specular uniform!");

                // Attenuation
                shader->hasUniform(prefix + ".constant")
                    ? shader->setFloat(prefix + ".constant", lightData->getConstant())
                    : Logger::warn("[LightManager::uploadLights] shader has no constant uniform!");

                shader->hasUniform(prefix + ".linear")
                    ? shader->setFloat(prefix + ".linear", lightData->getLinear())
                    : Logger::warn("[LightManager::uploadLights] shader has no linear uniform!");

                shader->hasUniform(prefix + ".quadratic")
                    ? shader->setFloat(prefix + ".quadratic", lightData->getQuadratic())
                    : Logger::warn("[LightManager::uploadLights] shader has no quadratic uniform!");

                // Spotlight
                shader->hasUniform(prefix + ".cutOff")
                    ? shader->setFloat(prefix + ".cutOff", lightData->getCutOff())
                    : Logger::warn("[LightManager::uploadLights] shader has no cutOff uniform!");

                shader->hasUniform(prefix + ".outerCutOff")
                    ? shader->setFloat(prefix + ".outerCutOff", lightData->getOuterCutOff())
                    : Logger::warn("[LightManager::uploadLights] shader has no outerCutOff uniform!");

                shader->hasUniform(prefix + ".type")
                    ? shader->setInt(prefix + ".type", static_cast<int>(light->getType()))
                    : Logger::warn("[LightManager::uploadLights] shader has no type uniform!");
            }
            if (shader->hasUniform("activeLightCount"))
                shader->setUint("activeLightCount", m_totalLightCount);
            else
                Logger::warn("[LightManager::uploadLights] shader has no activeLightCount uniform!");
        }
    }

    void LightManager::addStaticLight(const std::weak_ptr<Light> &light) {
        if (light.expired()) {
            Logger::warn("[LightManager::addStaticLight] light is null!");
            return;
        }
        m_staticLightsVec.push_back(light);
        Logger::info("Static Light added!");
        m_staticLightsDirty = true;

        calculateLightCount();
    }

    void LightManager::addStationaryLight(const std::weak_ptr<Light> &light) {
        if (light.expired()) {
            Logger::warn("[LightManager::addStationaryLight] light is null!");
            return;
        }

        if (m_lastIdxStationary < MAX_STATIONARY_LIGHTS) {
            if (m_freeStationaryIndices.empty()) {
                m_stationaryLights[m_lastIdxStationary++] = light;
            } else {
                // don't increment the light count because reused indices
                m_stationaryLights[m_freeStationaryIndices.top()] = light;
                m_freeStationaryIndices.pop();
            }
            m_stationaryLightsDirty = true;
            Logger::info("Stationary Light added!");
        } else {
            Logger::warn("You have reached the maximum stationary light count!");
        }
        calculateLightCount();
    }

    void LightManager::addDynamicLight(const std::weak_ptr<Light> &light) {
        if (light.expired()) {
            Logger::warn("[LightManager::addDynamicLight] light is null!");
            return;
        }

        if (m_lastIdxDynamic < MAX_DYNAMIC_LIGHTS) {
            if (m_freeDynamicIndices.empty()) {
                m_dynamicLights[m_lastIdxDynamic++] = light;
            } else {
                // don't increment the light count because reused indices
                m_dynamicLights[m_freeDynamicIndices.top()] = light;
                m_freeDynamicIndices.pop();
            }
            m_dynamicLightsDirty = true;
            Logger::info("Dynamic Light added!");
        } else {
            Logger::warn("You have reached the maximum dynamic light count!");
        }
        calculateLightCount();
    }

    void LightManager::calculateLightCount() {
        m_totalLightCount = m_staticLightsVec.size() +
            (m_lastIdxStationary - m_freeStationaryIndices.size()) +
                (m_lastIdxDynamic - m_freeDynamicIndices.size());
    }

    void LightManager::cleanupExpiredLights()
    {
        if (m_staticLightsDirty) {
            m_staticLightsVec.erase(
            std::remove_if(m_staticLightsVec.begin(), m_staticLightsVec.end(),
                [](const std::weak_ptr<Light>& wptr) { return wptr.expired(); }),
            m_staticLightsVec.end());
            m_staticLightsDirty = false;
        }

        if (m_stationaryLightsDirty) {
            // clean expired pointers from stationary lights from the array
            for (size_t i = 0; i < MAX_STATIONARY_LIGHTS; i++) {
                if (m_stationaryLights[i].expired()) {
                    m_freeStationaryIndices.push(i);
                    m_stationaryLights[i].reset();
                }
            }
            m_stationaryLightsDirty = false;
        }

        if (m_dynamicLightsDirty) {
            // clean expired pointers from dynamic lights from the array
            for (size_t i = 0; i < MAX_DYNAMIC_LIGHTS; i++) {
                if (m_dynamicLights[i].expired()) {
                    m_freeDynamicIndices.push(i);
                    m_dynamicLights[i].reset();
                }
            }
            m_dynamicLightsDirty = false;
        }

        calculateLightCount();

        // reset flags
        m_stationaryLightsDirty = false;
        m_dynamicLightsDirty = false;
    }
}
