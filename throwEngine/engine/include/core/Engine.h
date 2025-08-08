#pragma once
#include "core/Window.h"
#include "core/File.h"
#include "graphics/Renderer/Renderer.h"
#include "graphics/Camera/CameraController.h"
#include "graphics/Shaders/ShaderManager.h"
#include "Scene/SceneObjectFactory.h"
#include "graphics/Grid/GridSystem.h"
#include "ImGui/ImGuiLayer.h"

static float SCR_WIDTH = 1540.f;
static float SCR_HEIGHT = 860.f;


// Forward Declarations ON

namespace Graphics
{
	class Transform;
	class MeshFactory;
	class RenderData;
}

namespace Input
{
	class Input;
};

namespace LIGHTING
{
	class Light;
	class LightData;
	class LightManager;
}

namespace SCENE
{
	class Scene;
};

namespace Graphics { class TextureManager; };

namespace Graphics
{
	class MaterialLibrary;
};

namespace SHADER
{
	class GLShaderProgram;
}

// Forward Declarations OF


namespace core
{

	class Engine {
	public:
		Engine() = default;
		~Engine();

		[[nodiscard]] bool run();
		[[nodiscard]] bool initResources();
		void initPointerObjects();

	private:

		std::unique_ptr<Window> m_window;

		Input::InputContext dataInputContext;
		
		std::shared_ptr<Graphics::MeshData3D> meshData3D;

		std::shared_ptr<Graphics::Transform> transformationManager;

		std::shared_ptr<Graphics::MaterialLibrary> m_MaterialLibraryPtr;

		std::shared_ptr<Graphics::Camera> cameraManager;
		std::unique_ptr<Graphics::CameraInputSystem> cameraInput;

		std::unique_ptr<SCENE::SceneObjectFactory> sceneObjectFactory;

		std::shared_ptr<Graphics::RenderData> renderData;

		std::unique_ptr<Graphics::Renderer> rendererManager;

		std::shared_ptr<SHADER::ShaderManager> shaderManager;

		std::shared_ptr<SCENE::Scene> scene;

		std::shared_ptr<Graphics::TextureManager> textureManager;

		std::shared_ptr<LIGHTING::LightManager> lightManager;

		std::unique_ptr<GRID::GridSystem> m_gridSystem;

		std::unique_ptr<ENGINE::UI::ImGuiLayer> m_imGuiLayer;

		void initWindow();

		void initCallBack() noexcept;

		void initShader();

		void initTexture();

		void initMaterial();

		void initCamera();

		void initTransform();

		void initRender();

		void initMesh();

		void initImGui();

		void initScene();

		void initGrid();

		void initLighting();

		void OpenGLSetUpResources() noexcept;
		void OpenGLRenderStuff()    noexcept;
		void glfwRenderEventStuff() const  noexcept;

		bool m_RequestShutdown = false;
	};
};

