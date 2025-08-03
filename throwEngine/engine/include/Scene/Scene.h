#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include "glm/ext.hpp"
#include "Input/InputContext.h"
#include <unordered_map>
#include "graphics/Grid/GridSystem.h"

namespace GLgraphics
{
	class RenderData;
	class MeshData3D;
};

namespace LIGHTING
{
	class Light;
	class LightData;
	class LightManager;
}

namespace SHADER
{
	class ShaderManager;
}

namespace MATERIAL
{
	class MaterialLibrary;
	struct Material;
}

namespace Input { class IInputComponent; };

namespace SCENE
{
	// Forward declarations
	class SceneObject;
	class SceneObjectFactory;

	class Scene
	{
	public:
		Scene(const std::shared_ptr<GLgraphics::MeshData3D> &data3D, Input::InputContext context);

		void setSceneObjectFactoryPointer(SceneObjectFactory* factory) { sceneObjectFactory = factory; };
		[[nodiscard]] SceneObjectFactory* getSceneObjectFactory() const { return sceneObjectFactory; };

		[[nodiscard]] bool SetUpResources(const std::shared_ptr<MATERIAL::MaterialLibrary>& library);

		void initGrid(const std::shared_ptr<GLgraphics::RenderData>& renderData) const;
		void setOwnershipGridSystemToScene(std::unique_ptr<GRID::GridSystem>& gridSystem) { m_gridSystem = std::move(gridSystem); };

		void debugDrawing(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);
		void drawGrid(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);
		void drawAllObjects(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData);

		[[nodiscard]] std::vector<std::shared_ptr<SceneObject>>& getSceneObjectVec() { return m_sceneObjectsVec; };
		[[nodiscard]] std::shared_ptr<SceneObject> getObjectWithNameFromMap(const std::string &name);

		void updateInputComponents();

		[[nodiscard]] uint32_t uniqueObjectIDGenerator();

		void createObjectProperties(const std::shared_ptr<SceneObject>& sceneObject);
		void markObjectsToDelete(const std::string& name);
		void cleanUpMarkedObjects();

	private:
		std::shared_ptr<GLgraphics::MeshData3D> meshData3D;

		Input::InputContext inputContext;

		std::shared_ptr<GLgraphics::RenderData> m_renderData;

		std::unique_ptr<GRID::GridSystem> m_gridSystem;

		std::vector<std::shared_ptr<Input::IInputComponent>> m_inputComponents;

		SceneObjectFactory* sceneObjectFactory = nullptr;

		/************************************************************/

		bool m_needsCleanUp = false;

		std::unordered_map<uint32_t, std::shared_ptr<SceneObject>> m_sceneObjectsMap;

		std::unordered_map<std::string, uint32_t > m_objectIDMap;

		std::vector<std::shared_ptr<SceneObject>> m_sceneObjectsVec;

		std::stack<uint32_t> m_freeIDs;
	};
}