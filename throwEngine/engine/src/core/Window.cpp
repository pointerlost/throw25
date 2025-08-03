#include "core/Window.h"


namespace core {

	Window::Window() :
		width(1580), height(820), title("why do we fall, so we can learn to pick ourselves up."), monitor(nullptr), share(nullptr), mode(nullptr)
	{
		window = nullptr;
	}

	bool Window::initResources()
	{
		if (!glfwInit()) {
			std::cerr << "glfwInit failed!" << std::endl;
			return false;
		}

		SetUpGLFWmonitorVidmode();
		SetUpGLFWContext();
		
		window = glfwCreateWindow(mode->width, mode->height, title, monitor, NULL);

		if (!window) {
			std::cerr << "glfwCreateWindow failed!" << std::endl;
			return false;
		}

		// GLFW doesn't leave the window's OpenGL context current when glfwCreateWindow() succeeds!
		glfwMakeContextCurrent(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//glfwSetCursorPos(window, width / 2, height / 2);

		loadGlad();

		/*std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		int profile;
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
		if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
			std::cout << "Core profile\n";
		else if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
			std::cout << "Compatibility profile\n";*/

		return true;
	}

	GLFWwindow* Window::getGLFWwindow()
	{
		return window;
	}

	int Window::getWidth() const
	{
		return width;
	}

	int Window::getHeight() const
	{
		return height;
	}

	void Window::SetUpGLFWmonitorVidmode()
	{
		monitor = glfwGetPrimaryMonitor();

		if (!monitor) {
			std::cerr << "monitor can't be nullptr for fullscreen mode (from core::Window) !\n";
		}

		mode = glfwGetVideoMode(monitor);

		if (!mode) {
			std::cerr << "vidmode can't be nullptr for fullscreen mode (from core::Window) !\n";
		}

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	}

	void Window::SetUpGLFWContext()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}

	bool Window::loadGlad()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "gladLoadGLLoader failed!" << std::endl;
			return false;
		}

		return true;
	}

}