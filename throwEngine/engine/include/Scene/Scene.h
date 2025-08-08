#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include "glm/ext.hpp"
#include "Input/InputContext.h"
#include <unordered_map>
#include "graphics/Grid/GridSystem.h"

namespace Graphics
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

namespace Graphics
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
		Scene(const std::shared_ptr<Graphics::MeshData3D> &data3D, Input::InputContext context);

		void setSceneObjectFactoryPointer(SceneObjectFactory* factory) { sceneObjectFactory = factory; };
		[[nodiscard]] SceneObjectFactory* getSceneObjectFactory() const { return sceneObjectFactory; };

		void initGrid(const std::shared_ptr<Graphics::RenderData>& renderData) const;
		void setOwnershipGridSystemToScene(std::unique_ptr<GRID::GridSystem>& gridSystem) { m_gridSystem = std::move(gridSystem); };

		void debugDrawing(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<Graphics::RenderData>& renderData);
		void drawGrid(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<Graphics::RenderData>& renderData);
		void drawAllObjects(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<Graphics::RenderData>& renderData);

		[[nodiscard]] std::vector<std::shared_ptr<SceneObject>>& getSceneObjectVec() { return m_sceneObjectsVec; };
		[[nodiscard]] const std::shared_ptr<SceneObject>& getObjectWithNameFromMap(const std::string &name) const;

		void updateInputComponents();

		[[nodiscard]] uint32_t uniqueObjectIDGenerator();

		void createObjectProperties(const std::shared_ptr<SceneObject>& sceneObject);
		void markToBeDeleted(const std::string& name);
		void cleanUp();
		void destroyObject(const std::string& name);

	private:
		std::shared_ptr<Graphics::MeshData3D> meshData3D;

		Input::InputContext inputContext;

		std::unique_ptr<GRID::GridSystem> m_gridSystem;

		std::vector<std::shared_ptr<Input::IInputComponent>> m_inputComponents;

		SceneObjectFactory* sceneObjectFactory = nullptr;

		/************************************************************/

		/*  This Scene object addition and deletion system written higher performance */

		// Maps unique object IDs to their shared_ptr for fast access
		std::unordered_map<uint32_t, std::shared_ptr<SceneObject>> m_sceneObjectsMap;

		// Maps object names to their index in the scene vector
		std::unordered_map<std::string, uint32_t> m_nameIndexHash;

		// Stores all scene objects, Some entries might be nullptr after deletion, so check before use.
		std::vector<std::shared_ptr<SceneObject>> m_sceneObjectsVec;

		// Stack of available (free) indices in the vector - for reusing slots
		std::stack<uint32_t> m_freeIndicesStack;

		// to use free IDs store in the stack
		std::stack<uint32_t> m_freeIDs;

		// keep items that need to be deleted until the end of the frame (to avoid crashes)
		std::stack<std::string> m_markForDeletion;
	};
}