#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input/InputContext.h"

namespace CAMERA
{
	class Camera;

	class CameraInputSystem
	{
	public:
		CameraInputSystem(Camera* cam, Input::InputContext context);

		void processInput(GLFWwindow* window);

		void setUpDataContext();

	private:
		Camera* camera;

		Input::InputContext dataContext;

		bool m_lockCamera = false;
	};
}