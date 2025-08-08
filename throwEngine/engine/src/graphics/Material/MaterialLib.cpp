#include "graphics/Material/MaterialLib.h"

#include "core/File.h"

#include "graphics/Textures/Textures.h"

#include "core/Config.h"

#include "core/Logger.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

static auto& file = core::File::get();

namespace Graphics
{
    bool MaterialLibrary::createMaterials(const std::string& filePath, Graphics::TextureManager& textureManager)
    {
        std::string content = file.readFromFile(filePath);

        if (content.empty()) {
            Logger::warn("[MaterialLibrary::readAndParseJSON] content is empty!\n");
            return false;
        }

        json parsed = json::parse(content);

        if (!parsed.contains("materials") || !parsed["materials"].is_array()) {
            Logger::warn("[MaterialLibrary::readAndParseJSON] Invalid material file structure!\n");
            return false;
        }

        for (auto& mat : parsed["materials"]) {
            std::string name = mat.value("name", "unnamed");

            const std::vector<float> ambient = mat.value("ambient", std::vector<float>());
            const std::vector<float> diffuse = mat.value("diffuse", std::vector<float>());
            const std::vector<float> specular = mat.value("specular", std::vector<float>());
            const float shininess = mat.value("shininess", float());

            const std::vector<std::string> diffuseTexName = mat.value("diffuseTexture", std::vector<std::string>());
            const std::vector<std::string> specularTexName = mat.value("specularTexture", std::vector<std::string>());

            if (ambient.size() != 3 || diffuse.size() != 3 || specular.size() != 3) {
                Logger::warn("Material \"" + name + "\" has incorrect color vector sizes.");
                continue;
            }

            auto material = std::make_shared<Material>();

            material->m_name = name;

            for (int i = 0; i < ambient.size(); i++) {
                material->m_ambient[i] = ambient[i];
                material->m_diffuse[i] = diffuse[i];
                material->m_specular[i] = specular[i];
            }

            material->m_shininess = shininess;

            if (!diffuseTexName.empty()) {
                std::string path = std::string(ASSETS_DIR) + "/" + diffuseTexName[0];
                textureManager.load(diffuseTexName[0], path);
                material->m_diffuseTexture = textureManager.getTextureWithName(diffuseTexName[0]);
            }

            if (!specularTexName.empty()) {
                std::string path = std::string(ASSETS_DIR) + "/" + specularTexName[0];
                textureManager.load(specularTexName[0], path);
                material->m_specularTexture = textureManager.getTextureWithName(specularTexName[0]);
            }

            if (!m_materials.contains(name))
                m_materials.emplace(name, material);
        }
        return true;
    }

    std::shared_ptr<Graphics::Material> MaterialLibrary::getMaterialByName(const std::string &name) {
        if (!m_materials.contains(name)) {
            Logger::warn("Material \"" + name + "\" not found! but returning fallback\n");
            // return default material
            return getDefaultMaterial();
        }
        return m_materials.at(name);
    }

    std::shared_ptr<Material> MaterialLibrary::getDefaultMaterial() {
        const auto fallback = std::make_shared<Material>();
        fallback->m_name = "default";
        fallback->m_ambient = glm::vec4(0.1f);
        fallback->m_diffuse = glm::vec4(1.0f);
        fallback->m_specular = glm::vec4(1.0f);
        fallback->m_shininess = 8.0f;
        fallback->m_diffuseTexture = nullptr;
        fallback->m_specularTexture = nullptr;

        Logger::info("Default Material added!");
        return fallback;
    }
}
