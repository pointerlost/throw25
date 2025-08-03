#include "graphics/Camera/CameraController.h"

#include "graphics/Camera/Camera.h"

#include "Input/Input.h"

#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace CAMERA
{
	CameraInputSystem::CameraInputSystem(Camera* cam, Input::InputContext context)
		: camera(cam), dataContext(context)
	{
		DEBUG_PTR(*camera);
		setUpDataContext();
	}

	void CameraInputSystem::processInput(GLFWwindow* window)
	{
		if (Input::g_IsKeyDown[GLFW_KEY_ESCAPE])
		{
			m_lockCamera = !m_lockCamera;
			glfwSetInputMode(window, GLFW_CURSOR, m_lockCamera ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		}

		if (m_lockCamera) return;

		dataContext.m_currentTime = glfwGetTime();

		if (Input::g_IsKeyHeld[GLFW_KEY_A]) {
			
			camera->setCameraPosition(-dataContext.m_rotationSpeed, camera->getCamFront(), camera->getCamUp());
			
			//std::cout << "'A' pressed!" << "\n";
		}

		if (Input::g_IsKeyHeld[GLFW_KEY_D]) {

			camera->setCameraPosition(dataContext.m_rotationSpeed, camera->getCamFront(), camera->getCamUp());
			
			//std::cout << "'D' pressed!" << "\n";
		}

		if (Input::g_IsKeyHeld[GLFW_KEY_W]) {

			camera->setCameraPosition(dataContext.m_rotationSpeed, camera->getCamFront());
			
			//std::cout << "'W' pressed!" << "\n";
		}

		if (Input::g_IsKeyHeld[GLFW_KEY_S]) {

			camera->setCameraPosition(-dataContext.m_rotationSpeed, camera->getCamFront());
			
			//std::cout << "'S' pressed!" << "\n";
		}

		// JUMPING STATE
		if (Input::g_IsKeyDown[GLFW_KEY_SPACE] && !dataContext.m_isJumping)
		{
			camera->setIsCameraDirty(true);

			dataContext.m_isJumping = true;
			// set lastTime
			dataContext.m_lastTime = dataContext.m_currentTime;
			dataContext.m_firstVelocityY = 5.0f;
			dataContext.m_startY = camera->getCamFront().y;
		}
		if (dataContext.m_isJumping)
		{
			float deltaTime = 1.0 / 60.0f;
			dataContext.m_firstVelocityY += dataContext.m_gravity * deltaTime;
			float newY = camera->getCamFront().y + dataContext.m_firstVelocityY * deltaTime;

			float elapsed = dataContext.m_currentTime - dataContext.m_lastTime;

			if (elapsed < dataContext.m_jumpDuration) {

				float jumpY = dataContext.m_startY + dataContext.m_jumpHeight * sin((elapsed / dataContext.m_jumpDuration) * 3.14159f);
				glm::vec3 pos = camera->getCameraPosition();
				pos.y = jumpY;

				camera->setCameraPosition(pos);
			}
			else {

				glm::vec3 pos = camera->getCameraPosition();
				pos.y = dataContext.m_startY;
				camera->setCameraPosition(pos);

				dataContext.m_isJumping = false;
			}

		}

		/* *********************************** MOUSE INPUT STATEMENT ******************************************* */

		if (Input::mouseDeltaX != 0.0 || Input::mouseDeltaY != 0.0) {

			camera->setYaw(Input::mouseDeltaX * Input::sensitivity);
			camera->setPitch(Input::mouseDeltaY * Input::sensitivity);

			Input::resetMouseDelta();
		}

		/* ************************************ DIRTY CAMERA FLAG ********************************************* */

		if ( camera->getIsCameraDirty() )
		{
			camera->updateCamera();
		}

	}

	void CameraInputSystem::setUpDataContext()
	{
		dataContext.m_rotationSpeed = 0.015f;
		dataContext.m_delta = -90.0f;

		dataContext.m_isJumping = false;
		dataContext.m_jumpDuration = 1.0f;
		dataContext.m_jumpHeight = 1.0f;
		dataContext.m_gravity = -9.67;
		dataContext.m_currentTime = glfwGetTime();
		camera->setIsCameraDirty(false);
	}

}