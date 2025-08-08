#include "Scene/SceneObject.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Transformations/Transformations.h"

#include "graphics/Shaders/ShaderProgram.h"
#include "graphics/Shaders/BasicShader.h"

#include <graphics/Textures/Textures.h>

#include "graphics/Material/MaterialLib.h"

#include "graphics/Lighting/LightManager.h"

#include "graphics/Camera/Camera.h"

#include <core/Logger.h>
#include "core/Debug.h"
#include "core/Engine.h"
#include "core/Engine.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace SCENE {
    SceneObject::SceneObject(std::shared_ptr<Graphics::IMesh> mesh, std::string materialName)
        : m_mesh(mesh), m_transform(std::make_shared<Graphics::Transform>()), m_materialName(materialName) {
        DEBUG_PTR(m_transform);
        DEBUG_PTR(m_mesh);
    }

    bool SceneObject::validateRenderState(const std::shared_ptr<Graphics::RenderData> &renderData) {
        // shader is an interface
        const auto &iShader = m_shaderInterface;
        DEBUG_PTR(iShader);

        if (!iShader) {
            Logger::warn("[SceneObject] Shader Interface not found! Skipping draw.");
            return false;
        }

        if (!iShader->getGLShaderProgram()) {
            Logger::warn("[SceneObject::validateRenderState] getGLShaderProgram is nullptr!");
            return false;
        }

        const auto lightManager = renderData->getLightManager();
        DEBUG_PTR(lightManager);

        if (!lightManager) {
            Logger::warn("[SceneObject] LightManager missing! Skipping draw.");
            return false;
        }

        auto &material = m_materialInstance;
        DEBUG_PTR(material);

        if (!material) {
            Logger::info("[SceneObject] Material missing! Not skipping but assign default material!");
            material = renderData->getMaterialLib()->getDefaultMaterial();
            return false;
        }

        const auto textureManager = renderData->getTextureManager();
        DEBUG_PTR(textureManager);

        if (!textureManager) {
            Logger::warn("[SceneObject] TextureManager missing! Skipping draw.");
            return false;
        }

        const auto camera = renderData->getCamera();
        DEBUG_PTR(camera);

        if (!camera) {
            Logger::warn("[SceneObject] Camera missing! Skipping draw.");
            return false;
        }

        return true;
    }

    void SceneObject::initializeMaterial(const std::shared_ptr<Graphics::MaterialLibrary> &library) {
        // take a copy of material
        const auto base = library->getMaterialByName(m_materialName);
        // then create a material instance for a scene object
        m_materialInstance = std::make_shared<Graphics::Material>(*base);

        if (!m_materialInstance) {
            Logger::warn("[SceneObject::initializeMaterial] m_materialInstance is doesn't exist!");
        }
    }

    void SceneObject::setMaterialInstance(std::shared_ptr<Graphics::Material> instance) {
        // we are copying materials because we can change to without affect original material
        m_materialInstance = instance;
    }

    void SceneObject::setShaderInterface(std::shared_ptr<SHADER::IShader> shader) {
        if (!shader) {
            Logger::error("[ERROR] [SceneObject::setShaderInterface] Shader is nullptr!");
            return;
        }
        switch (shader->getType()) {
            case SHADER::ShaderType::BASIC:
                m_shaderInterface = shader;
                return;
            default:
                Logger::error("[ERROR] [SceneObject::setShaderInterface] Unknown Shader type!");
        }
    }

    void SceneObject::draw(const glm::mat4 &view, const glm::mat4 &projection,
                           const std::shared_ptr<Graphics::RenderData> &renderData) {
        // check nullptr and other debugging stuff
        if (!validateRenderState(renderData)) {
            Logger::warn("[ERROR] [SceneObject::draw]: " + m_objectName + " check up!!!");
            Logger::error("[ERROR] [SceneObject::draw] validateRenderState pointer initialization failed!");
            return;
        }

        const auto model = m_transform->getModelMatrix();
        const glm::vec3 cameraPos = renderData->getCamera()->getCameraPosition();

        // prepare shader and set uniforms
        prepareShader(model, view, projection, cameraPos, renderData);

        // draw per object
        m_mesh->drawMeshObject();
    }

    void SceneObject::prepareShader(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection,
                                    const glm::vec3 &cameraPos,
                                    const std::shared_ptr<Graphics::RenderData> &renderData) const {
        if (!m_shaderInterface) {
            Logger::warn("[ERROR] [SceneObject::prepareShader] Shader Interface not found! skipping..");
            return;
        }
        const auto &iShader = m_shaderInterface;
        iShader->setRenderDataObject(renderData);
        iShader->bind();

        const auto &material = m_materialInstance;
        const auto texture = renderData->getTextureManager();

        // bind material
        bindMaterialAndTextures(renderData);

        // Upload lights and set global ambient
        iShader->getGLShaderProgram()->setVec3("globalAmbient", renderData->getGlobalAmbient());

        renderData->getLightManager()->uploadLights(iShader->getGLShaderProgram());

        // Bind material and textures
        iShader->setMaterial(material);

        // Set matrices and camera position
        iShader->setMatrices(model, view, projection, cameraPos);
    }

    void SceneObject::bindMaterialAndTextures(const std::shared_ptr<Graphics::RenderData> &renderData) const {
        const auto texture = renderData->getTextureManager();
        const auto &material = m_materialInstance;

        if (material->m_diffuseTexture)
            texture->bind(material->m_diffuseTexture->glID, 0);
        if (material->m_specularTexture)
            texture->bind(material->m_specularTexture->glID, 1);
    }
}
