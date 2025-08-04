#pragma once
#include <string>
#include <memory>
#include <glm/ext.hpp>

#include <Input/InputContext.h>

#include "graphics/Lighting/Light.h"
#include "graphics/Renderer/RenderData.h"

namespace SHADER
{
	class IShader;
}

namespace GLgraphics
{
	class MeshData3D;
}

namespace LIGHTING
{
	class LightData;
	class LightManager;
}

namespace MATERIAL { class MaterialLibrary; };

namespace SCENE
{
	class Scene;
	class SceneObject;

	class SceneObjectFactory
	{
	public:
		explicit SceneObjectFactory(const std::shared_ptr<GLgraphics::RenderData>& renderData);
		~SceneObjectFactory();

		[[nodiscard]] std::shared_ptr<GLgraphics::MeshData3D> getMeshData() const;

		[[nodiscard]] std::shared_ptr<SceneObject> createCube(const glm::vec3& pos = glm::vec3(0.0), const std::string& materialName = "leather", bool visualLightObj = false) const;

		[[nodiscard]] std::shared_ptr<SceneObject> createSphere(const glm::vec3& pos = glm::vec3(0.0), const std::string& materialName = "marble", bool visualLightObj = false) const;

		[[nodiscard]] bool createPointLight(const std::string& materialName = "gold",
			const glm::vec3& position = glm::vec3(7.0),
			LIGHTING::LightMobility mobility = LIGHTING::LightMobility::Static) const;

		[[nodiscard]] bool createDirectionalLight(const std::string& materialName = "gold",
			const glm::vec3& position = glm::vec3(7.0),
			LIGHTING::LightMobility mobility = LIGHTING::LightMobility::Static) const;

		[[nodiscard]] bool createSpotLight(const std::string& materialName = "gold",
			const glm::vec3& position = glm::vec3(7.0),
			LIGHTING::LightMobility mobility = LIGHTING::LightMobility::Static) const;

		static inline Input::InputContext createInputContext() {
			Input::InputContext inputContext{};
			inputContext.m_rotationSpeed = 0.5f;
			inputContext.m_delta = 0.016f; // Assuming 60 FPS
			inputContext.m_angle = 0.0f;
			inputContext.m_radius = 45.0f;
			return inputContext;
		};

		static std::string generateName(const std::string& type);

		void setScene(const std::shared_ptr<Scene>& scene) { m_scene = scene; };
		std::shared_ptr<Scene>& getScene() { return m_scene; };

	private:
		struct Impl;
		std::unique_ptr<Impl> m_pImpl;

		std::shared_ptr<GLgraphics::RenderData> m_renderData;

		std::shared_ptr<SCENE::Scene> m_scene;
		
		void initBaseMeshes() const;
	};
}