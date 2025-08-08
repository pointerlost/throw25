#include "graphics/Lighting/LightManager.h"

#include "graphics/Shaders/ShaderProgram.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Lighting/Light.h"

#include <Scene/SceneObject.h>

#include "core/Logger.h"
#include "graphics/Transformations/Transformations.h"
#include "graphics/Lighting/Light.h"

#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace LIGHTING
{
    void LightManager::setLightUniforms(const std::shared_ptr<SHADER::GLShaderProgram> &shader, size_t index,
        const std::shared_ptr<Light>& light)
    {
        const auto lightData = light->getLightData();
        if (!lightData) {
            removeLight(light->getVisual()->getName());
            return;
        }
        const std::string prefix = "lights[" + std::to_string(index) + "]";
        const std::string name = light->getVisual()->getName();

        // Position and direction
        shader->hasUniform(prefix + ".position")
            ? shader->setVec3(prefix + ".position", lightData->getPosition())
            : Logger::warn("[LightManager::uploadLights] shader has no position uniform!" + name);

        shader->hasUniform(prefix + ".direction")
            ? shader->setVec3(prefix + ".direction", lightData->getDirection())
            : Logger::warn("[LightManager::uploadLights] shader has no direction uniform!" + name);

        // Colors
        shader->hasUniform(prefix + ".diffuse")
            ? shader->setVec3(prefix + ".diffuse", lightData->getDiffuse())
            : Logger::warn("[LightManager::uploadLights] shader has no diffuse uniform!" + name);

        shader->hasUniform(prefix + ".specular")
            ? shader->setVec3(prefix + ".specular", lightData->getSpecular())
            : Logger::warn("[LightManager::uploadLights] shader has no specular uniform!" + name);

        // Attenuation
        // shader->hasUniform(prefix + ".constant")
        //     ? shader->setFloat(prefix + ".constant", lightData->getConstant())
        //     : Logger::warn("[LightManager::uploadLights] shader has no constant uniform!");
        //
        // shader->hasUniform(prefix + ".linear")
        //     ? shader->setFloat(prefix + ".linear", lightData->getLinear())
        //     : Logger::warn("[LightManager::uploadLights] shader has no linear uniform!");
        //
        // shader->hasUniform(prefix + ".quadratic")
        //     ? shader->setFloat(prefix + ".quadratic", lightData->getQuadratic())
        //     : Logger::warn("[LightManager::uploadLights] shader has no quadratic uniform!");
        //
        // // Spotlight
        // shader->hasUniform(prefix + ".cutOff")
        //     ? shader->setFloat(prefix + ".cutOff", lightData->getCutOff())
        //     : Logger::warn("[LightManager::uploadLights] shader has no cutOff uniform!");
        //
        // shader->hasUniform(prefix + ".outerCutOff")
        //     ? shader->setFloat(prefix + ".outerCutOff", lightData->getOuterCutOff())
        //     : Logger::warn("[LightManager::uploadLights] shader has no outerCutOff uniform!");

        // shader->hasUniform(prefix + ".type")
        //     ? shader->setInt(prefix + ".type", static_cast<int>(light->getType()))
        //     : Logger::warn("[LightManager::uploadLights] shader has no type uniform!");

        shader->hasUniform("activeLightCount")
        ? shader->setUint("activeLightCount", getActiveLightCount())
        : Logger::warn("[LightManager::uploadLights] shader has no activeLightCount uniform!");
    }

    void LightManager::uploadLights(const std::shared_ptr<SHADER::GLShaderProgram>& shader)
    {
        if (!shader) {
            Logger::error("Shader is null in LightManager::uploadLights");
            return;
        }
        // bind shader
        shader->bind();

        for (size_t i = 0; i < m_lights.size(); ++i) {
            auto& light = m_lights[i];
            if (!light) {
                Logger::warn("[LightManager::uploadLights] Light nullptr!");

                return;
            }

            light->update(light);
            setLightUniforms(shader, i, light);
        }
    }

    bool LightManager::checkLightExists(const std::string &name) const {
        return m_lightMap.contains(name);
    }

    void LightManager::addLight(const std::shared_ptr<Light>& light) {
        const auto& name = light->getVisual()->getName();
        if (m_lightMap.contains(name)) {
            removeLight(name);
        }

        if (m_NullIndexStackOfLights.empty()) {
            m_lights.push_back(light);
            getFreeIndexWithName[name] = m_lights.size() - 1;
        } else {
            m_lights[m_NullIndexStackOfLights.top()] = light;
            getFreeIndexWithName[name] = m_NullIndexStackOfLights.top();
            m_NullIndexStackOfLights.pop();
        }
        m_lightMap[name] = light;
    }

    void LightManager::removeLight(const std::string& name) {
        const auto it = getFreeIndexWithName.find(name);
        if (it == getFreeIndexWithName.end()) return;

        uint32_t index = it->second;
        if (m_lights.size() > index && m_lights[index]) {
            std::cout << m_lights[index]->getLightData().use_count() << "\n";
            m_lights[index].reset();
            m_lightMap.erase(name);
            m_NullIndexStackOfLights.push(index);
            getFreeIndexWithName.erase(it);
        }
    }

    std::shared_ptr<Light> LightManager::getLight(const std::string &name) const {
        if (const auto it = m_lightMap.find(name); it != m_lightMap.end()) {
            return it->second;
        }
        Logger::warn("[LightManager::getLight] object: " + name + " nullptr!");
        return {};
    }
}
