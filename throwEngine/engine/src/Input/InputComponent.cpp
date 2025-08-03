#include "Input/InputComponent.h"

#include "graphics/GLTransformations/Transformations.h"

#include "Scene/Scene.h"

#include "Scene/SceneObject.h"

#include "Input/Input.h"

#include "graphics/Lighting/LightManager.h"

#include "graphics/Lighting/Light.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace Input
{

	CubeInputComponent::CubeInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context)
		: m_transformation(transform), m_dataContext(context)
	{
		DEBUG_PTR(m_transformation);

		setUpInputContext();
	}

	void CubeInputComponent::processInput(SCENE::Scene &scene)
	{
		// auto &sceneObjects = scene.getSceneObjectVector();
		//
		// for (const auto& obj : sceneObjects) {
		//
		// 	if (obj->getObjectName() == "cube_0")
		// 	{
		// 		m_transformation = obj->getTransform();
		//
		// 		if (Input::g_IsKeyHeld[GLFW_KEY_DOWN]) {
		// 			m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ 0.0f, -3.0f, 0.0f });
		// 			//std::cout << "'DOWN' pressed!" << "\n";
		// 		}
		// 		if (Input::g_IsKeyHeld[GLFW_KEY_UP]) {
		// 			m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ 0.0f, 3.0f, 0.0f });
		// 			//std::cout << "'UP' pressed!" << "\n";
		// 		}
		// 		if (Input::g_IsKeyHeld[GLFW_KEY_LEFT]) {
		// 			m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ -3.0f, 0.0f, 0.0f });
		// 			//std::cout << "'LEFT' pressed!" << "\n";
		// 		}
		// 		if (Input::g_IsKeyHeld[GLFW_KEY_RIGHT]) {
		// 			m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ 3.0f, 0.0f, 0.0f });
		// 			//std::cout << "'RIGHT' pressed!" << "\n";
		// 		}
		// 		if (Input::g_IsKeyHeld[GLFW_KEY_ENTER]) {
		// 			m_transformation->addRotation(m_dataContext.m_angle, glm::vec3{ 0.0f, 0.0f, 3.0f });
		// 			//std::cout << "'ENTER' pressed!" << "\n";
		// 			//std::cout << dataContext.m_angle++ << "\n";
		// 		}
		//
		// 		break;
		// 	}
		// }
	}

	void CubeInputComponent::setUpInputContext()
	{
		/* CALCULATION back later
		dataContext.m_currentTime = glfwGetTime();
		dataContext.m_delta = dataContext.m_currentTime - dataContext.m_lastTime;
		dataContext.m_angle = dataContext.m_delta * dataContext.m_rotationSpeed;
		dataContext.m_lastTime = dataContext.m_currentTime;
		*/

		m_dataContext.m_rotationSpeed = 1.0f;
		m_dataContext.m_delta = -90.0f;
		m_dataContext.m_angle = 0.5f;
	}


	CircleInputComponent::CircleInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context)
		: m_transformation(transform), m_dataContext(context)
	{
		DEBUG_PTR(m_transformation);

		setUpInputContext();
	}

	void CircleInputComponent::setUpInputContext()
	{
		m_dataContext.m_rotationSpeed = 0.3f;
		m_dataContext.m_delta = -90.0f;
		m_dataContext.m_angle = 0.5f;
	}

	/*****************************************************************************************************/
	/*****************************************************************************************************/

	LightInputComponent::LightInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context, std::shared_ptr<LIGHTING::Light>& myLight)
		: m_transformation(transform), m_dataContext(context), m_light(myLight)
	{
		DEBUG_PTR(m_transformation);

		setUpInputContext();
	}

	void LightInputComponent::processInput(SCENE::Scene& scene)
	{
		// const auto& lightManager = scene.getLightManager();
		//
		// if (!lightManager) {
		// 	Logger::warn("[SunInputComponent] lightManager returning nullptr.");
		// 	return;
		// }
		//
		// const std::vector<std::shared_ptr<LIGHTING::Light>> lightVec = lightManager->getLightsByVec();
		// if (lightVec.empty()) {
		// 	Logger::warn("[SunInputComponent] lightVec is empty.");
		// 	return;
		// }
		//
		// const uint32_t lightVecSize = lightVec.size();
		//
		// if (m_inputSelectors.size() != lightVecSize)
		// {
		// 	m_inputSelectors.resize(lightVecSize, false);
		// 	m_inputSelectors[m_activeLightComponentIdx] = true;
		// }
		//
		// changeInputObjectWithPress(lightVecSize);
		//
		// for (uint32_t i = 0; i < lightVecSize; ++i)
		// {
		// 	if (!m_inputSelectors[i] && lightVec[i] != m_light) continue;
		//
		// 	m_isRotating ? circularMotion(m_light) : moveOnPress(m_light);
		// }
	}

	void LightInputComponent::setUpInputContext()
	{
		m_dataContext.m_rotationSpeed = 0.5f;
		m_dataContext.m_delta = 0.016f;
		m_dataContext.m_angle = 0.0f;
		m_dataContext.m_radius = 45.0f;
	}

	void LightInputComponent::circularMotion(std::shared_ptr<LIGHTING::Light>& light)
	{
		m_dataContext.m_angle += m_dataContext.m_rotationSpeed * m_dataContext.m_delta;

		glm::vec3 newPos{};

		newPos.x = m_dataContext.m_radius * cos(m_dataContext.m_angle);
		newPos.z = m_dataContext.m_radius * sin(m_dataContext.m_angle);

		newPos.y = 10 + 60 * sin(m_dataContext.m_angle);

		m_transformation->setPosition(newPos);
	}

	void LightInputComponent::moveOnPress(std::shared_ptr<LIGHTING::Light>& light)
	{
		glm::vec3 objPosition = m_transformation->getPosition();

		glm::vec3 moveOffset{ 0.0 };
		const float moveSpeed = 2.0 * m_dataContext.m_delta;

		if (Input::g_IsKeyHeld[GLFW_KEY_KP_8]) moveOffset.z -= moveSpeed;
		if (Input::g_IsKeyHeld[GLFW_KEY_KP_2]) moveOffset.z += moveSpeed;
		if (Input::g_IsKeyHeld[GLFW_KEY_KP_4]) moveOffset.x -= moveSpeed;
		if (Input::g_IsKeyHeld[GLFW_KEY_KP_6]) moveOffset.x += moveSpeed;
		if (Input::g_IsKeyHeld[GLFW_KEY_KP_7]) moveOffset.y += moveSpeed;
		if (Input::g_IsKeyHeld[GLFW_KEY_KP_1]) moveOffset.y -= moveSpeed;

		if (glm::length(moveOffset) > 0.0) {
			const glm::vec3 newPos = objPosition + moveOffset;
			m_transformation->setPosition(newPos);
		}
	}

	void LightInputComponent::changeInputObjectWithPress(uint32_t lightVecSize)
	{
		if (Input::g_IsKeyPressed[GLFW_KEY_3] && m_inputSelectors[m_activeLightComponentIdx]) {
			m_isRotating = !m_isRotating;
		}

		if (Input::g_IsKeyDown[GLFW_KEY_F10] && m_inputSelectors[m_activeLightComponentIdx]) {
			m_inputSelectors[m_activeLightComponentIdx] = false;

			if (m_activeLightComponentIdx == 0) {
				m_activeLightComponentIdx = lightVecSize - 1;
			}
			else {
				--m_activeLightComponentIdx;
			}

			m_inputSelectors[m_activeLightComponentIdx] = true;
		}

		if (Input::g_IsKeyDown[GLFW_KEY_F11] && m_inputSelectors[m_activeLightComponentIdx]) {
			m_inputSelectors[m_activeLightComponentIdx] = false;

			if (m_activeLightComponentIdx == lightVecSize - 1) {
				m_activeLightComponentIdx = 0;
			}
			else {
				++m_activeLightComponentIdx;
			}

			m_inputSelectors[m_activeLightComponentIdx] = true;
		}
	}

	SphereInputComponent::SphereInputComponent(std::shared_ptr<GLgraphics::Transformations> transform, Input::InputContext context)
		: m_transformation(transform), m_dataContext(context)
	{
		setUpInputContext();
	}

	void SphereInputComponent::processInput(SCENE::Scene& scene)
	{
		// auto& sceneObjects = scene.getSceneObjectVector();
		//
		// uint32_t sceneObjectSize = sceneObjects.size();
		//
		// if (m_inputSelectors.size() != sceneObjectSize)
		// {
		// 	m_inputSelectors.resize(sceneObjectSize, false);
		// 	m_inputSelectors[m_activeLightComponentIdx] = true;
		// }
		//
		// changeInputObjectWithPress(sceneObjectSize);
		//
		// for (uint32_t i = 0; i < sceneObjectSize; ++i)
		// {
		// 	if (!sceneObjects[i]) continue;
		// 	if (!m_inputSelectors[i]) continue;
		//
		// 	m_transformation = sceneObjects[i]->getTransform();
		// }
	}

	void SphereInputComponent::setUpInputContext()
	{
		m_dataContext.m_rotationSpeed = 1.0f;
		m_dataContext.m_delta = -90.0f;
		m_dataContext.m_angle = 0.5f;
	}

	void SphereInputComponent::changeInputObjectWithPress(uint32_t lightVecSize)
	{
		if (Input::g_IsKeyDown[GLFW_KEY_F8] && m_inputSelectors[m_activeLightComponentIdx]) {
			m_inputSelectors[m_activeLightComponentIdx] = false;

			if (m_activeLightComponentIdx == 0) {
				m_activeLightComponentIdx = lightVecSize - 1;
			}
			else {
				--m_activeLightComponentIdx;
			}

			m_inputSelectors[m_activeLightComponentIdx] = true;
		}

		if (Input::g_IsKeyDown[GLFW_KEY_F9] && m_inputSelectors[m_activeLightComponentIdx]) {
			m_inputSelectors[m_activeLightComponentIdx] = false;

			if (m_activeLightComponentIdx == lightVecSize - 1) {
				m_activeLightComponentIdx = 0;
			}
			else {
				++m_activeLightComponentIdx;
			}

			m_inputSelectors[m_activeLightComponentIdx] = true;
		}
	}

}

