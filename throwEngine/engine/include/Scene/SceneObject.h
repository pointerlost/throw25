#pragma once
#include <glm/ext.hpp>
#include <memory>

namespace Graphics
{
	class RenderData;
	class IMesh;
	class Transform;
}

namespace SHADER
{
	class ShaderManager;
	class IShader;
	class Shader;
};

namespace Graphics
{
	class MaterialLibrary;
	struct Material;
};

namespace Input { class IInputComponent; };

namespace LIGHTING
{
	class Light;
	struct LightColor;
	enum struct LightMobility;
};

namespace SCENE
{
	class Scene;

	class SceneObject
	{
	public:
		SceneObject(std::shared_ptr<Graphics::IMesh> mesh, std::string  materialName);
		virtual ~SceneObject() = default;

		[[nodiscard]] bool validateRenderState(const std::shared_ptr<Graphics::RenderData>& renderData);

		void initializeMaterial(const std::shared_ptr<Graphics::MaterialLibrary>& library);
		void setMaterialInstance(std::shared_ptr<Graphics::Material> instance);
		[[nodiscard]] std::shared_ptr<Graphics::Material> getMaterialInstance() const { return m_materialInstance; };
		[[nodiscard]] std::string getMaterialName() const { return m_materialName; };

		void setShaderInterface(std::shared_ptr<SHADER::IShader> shader);
		[[nodiscard]] std::shared_ptr<SHADER::IShader> getShaderInterface() const { return m_shaderInterface; };

		void draw(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<Graphics::RenderData>& renderData);

		void setID(const uint32_t id) { m_objectID = id;   };
		[[nodiscard]] uint32_t getID() const  { return m_objectID; };

		void setName(const std::string& name) { this->m_objectName = name; };
		[[nodiscard]] const std::string& getName() const { return m_objectName; };

		void setInputComponent(const std::shared_ptr<Input::IInputComponent>& inputComponent) { m_inputComponent = inputComponent; };

		[[nodiscard]] std::shared_ptr<Input::IInputComponent> getInputComponent() const { return m_inputComponent; };

		[[nodiscard]] std::shared_ptr<Graphics::Transform>& getTransform() { return m_transform; };

		void markForDeletion() { m_markedForDeletion = true; };
		[[nodiscard]] bool isMarkedForDeletion() const { return m_markedForDeletion; };

	private:

		void prepareShader(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
			const glm::vec3& cameraPos, const std::shared_ptr<Graphics::RenderData>& renderData) const;

		void bindMaterialAndTextures(const std::shared_ptr<Graphics::RenderData>& renderData) const;

		std::shared_ptr<Graphics::IMesh> m_mesh;
		std::shared_ptr<Graphics::Transform> m_transform;

		std::shared_ptr<Input::IInputComponent> m_inputComponent = nullptr;
		std::shared_ptr<SHADER::IShader> m_shaderInterface = nullptr;

		std::string m_objectName;
		std::string m_materialName;

		std::shared_ptr<Graphics::Material> m_materialInstance;

		bool m_markedForDeletion = false;

		uint32_t m_objectID = -1;
	};
}
