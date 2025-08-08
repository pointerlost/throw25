#include "Scene/Scene.h"

#include <Scene/SceneObject.h>

#include <graphics/Mesh/MeshFactory.h>

#include "graphics/Renderer/RenderData.h"

#include "graphics/Shaders/ShaderProgram.h"

#include "Scene/SceneObjectFactory.h"

#include "graphics/Grid/GridSystem.h"

#include <graphics/Transformations/Transformations.h>

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
	Scene::Scene(const std::shared_ptr<Graphics::MeshData3D> &data3D, Input::InputContext context)
		:meshData3D(data3D), inputContext(context)
	{
		DEBUG_PTR(meshData3D);
	}

	void Scene::debugDrawing(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<Graphics::RenderData>& renderData)
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

	void Scene::initGrid(const std::shared_ptr<Graphics::RenderData>& renderData) const
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

	void Scene::drawGrid(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<Graphics::RenderData>& renderData)
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

	void Scene::drawAllObjects(const glm::mat4& view, const glm::mat4& projection, const std::shared_ptr<Graphics::RenderData>& renderData)
	{
		if (!renderData) {
			Logger::warn("[Scene::drawAllObjects] renderData is nullptr!");
			return;
		}

		for (const auto& obj : m_sceneObjectsVec)
		{
			if (!obj) {
				Logger::error("[Scene::drawAllObjects] a scene object was nullptr, skipping draw!");
				continue;
			}
			obj->draw(view, projection, renderData);
		}
		cleanUp();
	}

	uint32_t Scene::uniqueObjectIDGenerator()
	{
		static uint32_t uniqueID = 0;
		return uniqueID++;
	}

	void Scene::createObjectProperties(const std::shared_ptr<SceneObject>& sceneObject)
	{
		uint32_t uniqueID{};
		if (!sceneObject) {
			Logger::warn("[Scene::createObjectProperties] Null SceneObject passed — skipping creation.");
			return;
		}

		// Reuse an ID if available
		if (m_freeIDs.empty()) {
			uniqueID = uniqueObjectIDGenerator();
		} else {
			uniqueID = m_freeIDs.top();
			m_freeIDs.pop();
		}

		// assign uniqueID to object itself
		sceneObject->setID(uniqueID);

		uint32_t assignedIndex{};

		if (m_freeIndicesStack.empty()) {
			// No free slots use push_back
			assignedIndex = static_cast<uint32_t>(m_sceneObjectsVec.size());
			m_sceneObjectsVec.push_back(sceneObject);
		} else {
			// Reuse a free slot
			assignedIndex = m_freeIndicesStack.top();
			m_sceneObjectsVec[assignedIndex] = sceneObject;
			m_freeIndicesStack.pop();
		}

		// Save the index for fast lookup by name
		m_nameIndexHash[sceneObject->getName()] = assignedIndex;

		// Save in ID->Object map
		m_sceneObjectsMap[uniqueID] = sceneObject;
	}

	void Scene::markToBeDeleted(const std::string &name) {
		m_markForDeletion.push(name);
	}

	void Scene::cleanUp() {
		while (!m_markForDeletion.empty()) {
			destroyObject(m_markForDeletion.top());
			m_markForDeletion.pop();
		}
	}

	void Scene::destroyObject(const std::string &name) {

		if (!m_nameIndexHash.contains(name)) {
			Logger::warn("Attempted to destroy object with invalid name: " + name);
			return;
		}
		// Get the object's index in the vector from the name hash
		const auto index = m_nameIndexHash[name];

		if (index >= m_sceneObjectsVec.size() && !m_sceneObjectsVec[index]) return;

		// Get the object's ID before resetting
		const auto id = m_sceneObjectsVec[index]->getID();

		// Delete the object from the vector (free the slot)
		m_sceneObjectsVec[index].reset();

		// Recycle the freed index for reuse
		m_freeIndicesStack.push(index);

		// Recycle the freed ID for reuse
		m_freeIDs.push(id);

		// Remove from the ID-based map
		if (m_sceneObjectsMap.contains(id)) {
			m_sceneObjectsMap.erase(id);
		} else {
			Logger::warn("No object found for this ID: " + std::to_string(id));
			return;
		}

		// Remove from the name -> index map
		m_nameIndexHash.erase(name);
	}

	const std::shared_ptr<SceneObject> & Scene::getObjectWithNameFromMap(const std::string &name) const {
		if (m_sceneObjectsMap.contains(m_nameIndexHash.at(name))) {
			return m_sceneObjectsMap.at(m_nameIndexHash.at(name));
		}
		return {};
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