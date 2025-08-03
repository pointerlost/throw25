#pragma once
#include <glm/ext.hpp>
#include <memory>

namespace GLgraphics
{
	class RenderData;
	class IMesh;
	class Transformations;
}

namespace SHADER
{
	class ShaderManager;
	class IShader;
	class Shader;
};

namespace MATERIAL
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
		SceneObject(std::shared_ptr<GLgraphics::IMesh> mesh, std::string  materialName);
		virtual ~SceneObject() = default;

		[[nodiscard]] bool validateRenderState(const std::shared_ptr<GLgraphics::RenderData>& renderData);

		void initializeMaterial(const std::shared_ptr<MATERIAL::MaterialLibrary>& library);
		void setMaterialInstance(std::shared_ptr<MATERIAL::Material> instance);
		[[nodiscard]] std::shared_ptr<MATERIAL::Material> getMaterialInstance() const { return m_materialInstance; };
		[[nodiscard]] std::string getMaterialName() const { return m_materialName; };

		void setShaderInterface(const std::shared_ptr<SHADER::IShader> shader);
		[[nodiscard]] std::shared_ptr<SHADER::IShader> getShaderInterface() const { return m_shaderInterface; };

		void draw(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);

		void setID(const uint32_t id) { m_objectID = id;   };
		[[nodiscard]] uint32_t getID() const  { return m_objectID; };

		void setName(const std::string& name) { this->m_objectName = name; };
		[[nodiscard]] const std::string& getName() const { return m_objectName; };

		void setInputComponent(const std::shared_ptr<Input::IInputComponent>& inputComponent) { m_inputComponent = inputComponent; };

		[[nodiscard]] std::shared_ptr<Input::IInputComponent> getInputComponent() const { return m_inputComponent; };

		[[nodiscard]] std::shared_ptr<GLgraphics::Transformations>& getTransform() { return m_transform; };

		void markForDeletion() { m_markedForDeletion = true; };
		[[nodiscard]] bool isMarkedForDeletion() const { return m_markedForDeletion; };

		void setMobility(LIGHTING::LightMobility mobility) { m_mobility = mobility; };
		[[nodiscard]] LIGHTING::LightMobility getMobility() const { return m_mobility; };

	private:

		void prepareShader(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection,
			const glm::vec3& cameraPos, const std::shared_ptr<GLgraphics::RenderData>& renderData) const;

		void bindMaterialAndTextures(const std::shared_ptr<GLgraphics::RenderData>& renderData) const;

		std::shared_ptr<GLgraphics::IMesh> m_mesh;
		std::shared_ptr<GLgraphics::Transformations> m_transform;

		std::shared_ptr<Input::IInputComponent> m_inputComponent = nullptr;
		std::shared_ptr<SHADER::IShader> m_shaderInterface = nullptr;

		std::string m_objectName;
		std::string m_materialName;

		std::shared_ptr<MATERIAL::Material> m_materialInstance;

		bool m_markedForDeletion = false;

		uint32_t m_objectID = -1;

		// if it is a light object we are this object using for lighting
		LIGHTING::LightMobility m_mobility;
	};
}
