#pragma once
#include <memory>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "graphics/Textures/Textures.h"

// Forward declaration
namespace Graphics { class TextureManager; };

using json = nlohmann::json;

namespace Graphics
{

	struct Material
	{
		std::string m_name;

		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;

		std::shared_ptr<Graphics::Texture> m_diffuseTexture;
		std::shared_ptr<Graphics::Texture> m_specularTexture;

		Material() = default;
	};

	class MaterialLibrary {
	public:
		MaterialLibrary() = default;
		~MaterialLibrary() = default;

		[[nodiscard]] bool createMaterials(const std::string& filePath, Graphics::TextureManager& textureManager);

		std::shared_ptr<Graphics::Material> getMaterialByName(const std::string& name);

		std::shared_ptr<Material> getDefaultMaterial();

	private:
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
	};
}