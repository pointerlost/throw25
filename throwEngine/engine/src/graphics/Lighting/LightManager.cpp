#include "graphics/Lighting/LightManager.h"

#include "graphics/Shaders/ShaderProgram.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Lighting/Light.h"

#include <Scene/SceneObject.h>

#include "core/Logger.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace LIGHTING
{
    void LightManager::setLightUniforms(const std::shared_ptr<SHADER::GLShaderProgram> &shader, const std::string& prefix,
        const std::shared_ptr<Light>& light) const
    {
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
            : Logger::warn("[LightManager::uploadLights] shader has no diffuse uniform!" + light->getVisual()->getName());

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

    void LightManager::uploadLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader) const
    {
        if (!shader) {
            Logger::error("Shader is null in LightManager::uploadLights");
            return;
        }
        // bind shader
        shader->bind();

        size_t currentIndex = 0;

        auto uploadLightList = [&](const auto& lightList) {
            for (const auto& sharedLight : lightList) {
                if (auto& light = sharedLight) {
                    light->update(light);
                    const std::string prefix = "lights[" + std::to_string(currentIndex++) + "]";
                    setLightUniforms(shader, prefix, light);
                }
            }
        };

        uploadLightList(m_staticLightsVec);
        uploadLightList(m_stationaryLights);
        uploadLightList(m_dynamicLights);

        // set light count
        shader->hasUniform("activeLightCount")
        ? shader->setUint("activeLightCount", m_totalLightCount)
        : Logger::warn("[LightManager::uploadLights] shader has no activeLightCount uniform!");
    }

    void LightManager::addStaticLight(const std::shared_ptr<Light>& light) {
        if (!light || !light->getVisual()) {
            Logger::warn("[LightManager::addStaticLight] light is null or getVisual returning nullptr!");
            return;
        }
        m_staticLightsVec.push_back(light);
        m_staticLightsMap[light->getVisual()->getName()] = light;
        Logger::info("Static Light added!");
        m_staticLightsDirty = true;

        calculateLightCount();
    }

    void LightManager::addStationaryLight(const std::shared_ptr<Light> &light) {
        if (!light) {
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

    void LightManager::addDynamicLight(const std::shared_ptr<Light> &light) {
        if (!light) {
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
            std::ranges::remove_if(m_staticLightsVec,
            [](const std::shared_ptr<Light>& ptr) { return ptr ? ptr->getVisual()->isMarkedForDeletion() : true; }).begin(),
            m_staticLightsVec.end());
            m_staticLightsDirty = false;
        }

        if (m_stationaryLightsDirty) {
            // clean expired pointers from stationary lights from the array
            for (size_t i = 0; i < MAX_STATIONARY_LIGHTS; i++) {
                if (m_stationaryLights[i]) {
                    m_freeStationaryIndices.push(i);
                    m_stationaryLights[i].reset();
                }
            }
            m_stationaryLightsDirty = false;
        }

        if (m_dynamicLightsDirty) {
            // clean expired pointers from dynamic lights from the array
            for (size_t i = 0; i < MAX_DYNAMIC_LIGHTS; i++) {
                if (m_dynamicLights[i]) {
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
