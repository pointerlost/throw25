#include "Scene/Scene.h"

#include <Scene/SceneObject.h>

#include <graphics/Mesh/GLMeshFactory.h>

#include "graphics/Mesh/GLMeshCube3D.h"

#include "graphics/Mesh/GLMeshSphere3D.h"

#include "graphics/Mesh/GLMeshCircle3D.h"

#include "graphics/Mesh/GLMeshSquare3D.h"

#include "graphics/Mesh/GLMeshTriangle3D.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Shaders/ShaderProgram.h"

#include "Scene/SceneObjectFactory.h"

#include "graphics/Grid/GridSystem.h"

#include <graphics/GLTransformations/Transformations.h>

#include "graphics/Lighting/LightManager.h"

#include <graphics/Lighting/Light.h>

#include "graphics/Shaders/ShaderManager.h"

#include "graphics/Shaders/BasicShader.h"

#include "graphics/Material/MaterialLib.h"

#include "Input/InputComponent.h"

#include "graphics/Camera/Camera.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace SCENE
{
	Scene::Scene(const std::shared_ptr<GLgraphics::MeshData3D> &data3D, Input::InputContext context)
		:meshData3D(data3D), inputContext(context)
	{
		DEBUG_PTR(meshData3D);
	}

	void Scene::debugDrawing(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		//DEBUG MODE
		for (auto& obj : m_sceneObjectsVec) {
			try {
				if (obj) {
					obj->draw(view, projection, renderData);
				}
				else {
					throw std::runtime_error("trying to draw an object that returns nullptr!");
					// using throw can be costly (especially in per-frame loops)
				}
			}
			catch (const std::exception& e) {
				Logger::error("Error drawing object " + obj->getName() + ": " + e.what());
			}
		}
	}

	bool Scene::SetUpResources(const std::shared_ptr<MATERIAL::MaterialLibrary>& library)
    {
		if (!sceneObjectFactory) {
			Logger::error("Scene can't setup m_sceneObjectFactory is nullptr!");
		}

		(void)sceneObjectFactory->createSpotLight();

		(void)sceneObjectFactory->createPointLight();

		(void)sceneObjectFactory->createCube();

        (void)sceneObjectFactory->createCube();

        Logger::info("[Scene] SetUpResources successful!");
        return true;
    }

	void Scene::initGrid(const std::shared_ptr<GLgraphics::RenderData>& renderData) const
	{
		const auto shaderManager = renderData->getShaderManager();
		if (!shaderManager) {
			Logger::warn("[Scene::initGrid] ShaderManager is nullptr!");
			return;
		}

		auto gridShader = shaderManager->getShaderInterface("basic");
		if (!gridShader) {
			Logger::warn("[Warning] lightObjectShader not found, using fallback");
			gridShader = shaderManager->getShaderInterface("default");
			if (!gridShader) {
				Logger::error("[Error] Fallback shader not found!");
				return;
			}
		}
		
		if (!m_gridSystem) {
			Logger::warn("[Scene::initGrid] m_gridSystem returning nullptr");
			return;
		}
		
		// this function receives messages from the "Logger", no need to retrieve them again
		if (!m_gridSystem->getRenderer()->setGridShaderInterface(gridShader))
			return;

		Logger::info("[Scene::initGrid] successful!");
	}

	void Scene::drawGrid(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		if (const auto gridRenderer = renderData->getGridRenderer()) {
			if (const auto shader = gridRenderer->getGridShader()) {
				if (const auto shaderProgram = shader->getGLShaderProgram()) {
					shaderProgram->bind();
					shaderProgram->setMat4("u_view", view);
					shaderProgram->setMat4("u_projection", projection);
					gridRenderer->draw();
				}
			}
		}
	}

	void Scene::drawAllObjects(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<GLgraphics::RenderData>& renderData)
	{
		// I'll be here back later
		// drawGrid(view, projection, renderData);

		// cleanup marked objects to delete at the beginning
		cleanUpMarkedObjects();
		for (const auto& obj : m_sceneObjectsVec)
		{
			if (!obj) {
				Logger::error("[Scene::drawAllObjects] a scene object was nullptr, skipping draw!");
				continue;
			}
			obj->draw(view, projection, renderData);
		}
	}

	std::shared_ptr<SceneObject> Scene::getObjectWithNameFromMap(const std::string& name)
	{
		if (m_objectIDMap.contains(name)) {
			if (const auto it = m_sceneObjectsMap.find(m_objectIDMap[name]); (it != m_sceneObjectsMap.end()))
				return it->second;
		} else {
			Logger::warn("[Scene::getObjectWithNameFromMap]: Object '" + name + "' does not exist!");
		}
		return {};
	}

	uint32_t Scene::uniqueObjectIDGenerator()
	{
		static uint32_t uniqueID = 0;
		return uniqueID++;
	}

	void Scene::createObjectProperties(const std::shared_ptr<SceneObject>& sceneObject)
	{
		uint32_t uniqueID;
		if (!sceneObject) {
			Logger::warn("[Scene::createObjectProperties] Null SceneObject passed — skipping creation.");
			return;
		}
		if (m_freeIDs.empty()) {
			uniqueID = uniqueObjectIDGenerator();
		} else {
			uniqueID = m_freeIDs.top();  m_freeIDs.pop();
		}

		// assign uniqueID to object itself
		sceneObject->setID(uniqueID);

		// add a scene object with ID to map
		m_sceneObjectsMap[uniqueID] = sceneObject;

		// add a scene object as weak_ptr to vector
		m_sceneObjectsVec.push_back(sceneObject);

		// match name and ID for fast search and deletion
		m_objectIDMap[sceneObject->getName()] = uniqueID;
	}

	void Scene::markObjectsToDelete(const std::string& name)
	{
		uint32_t objectID{};
		// find object's id to remove it
		if (m_objectIDMap.contains(name)) {
			objectID = m_objectIDMap[name];
			m_objectIDMap.erase(name);
		} else {
			Logger::warn("[Scene::deleteObjectFromScene] Object '" + name + "' doesn't exist!");
			return;
		}

		// mark object for lazy deletion
		if (m_sceneObjectsMap.contains(objectID)) {
			m_sceneObjectsMap[objectID]->markForDeletion();
			m_needsCleanUp = true;
		} else {
			Logger::warn("[Scene::markObjectsToDelete]" + name + "doesn't exist in map!");
		}
	}

	// LAZY DELETION
	void Scene::cleanUpMarkedObjects()
	{
		if (!m_needsCleanUp) return;

		for (auto it = m_sceneObjectsMap.begin(); it != m_sceneObjectsMap.end(); ) {
			const auto& sceneObject = it->second;

			if (sceneObject->isMarkedForDeletion()) {
				uint32_t id = it->first;
				it = m_sceneObjectsMap.erase(it);
				m_freeIDs.push(id);
			} else {
				++it;
			}
		}

		// Cleanup marked pointers
		std::erase_if(m_sceneObjectsVec,
			[](const std::shared_ptr<SceneObject>& ptr) {
              return ptr ? ptr->isMarkedForDeletion() : false;
		});
	}

	void Scene::updateInputComponents()
	{
		if (m_inputComponents.empty()) {
			return;
		}

		for (const auto& component : m_inputComponents) {
			if (!component) {
				Logger::warn("[SceneObject] Input Component is nullptr, skipping update!");
				return;
			}
			component->processInput(*this);
		}
	}

}