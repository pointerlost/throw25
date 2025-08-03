#include "core/Engine.h"

#include "core/Config.h"

#include "core/Debug.h"

#include "CallBack/CallBack.h"

#include "graphics/Shaders/ShaderManager.h"

#include "graphics/GLTransformations/Transformations.h"

#include "graphics/Renderer/RenderData.h"

#include "graphics/Textures/Textures.h"

#include "graphics/Material/MaterialLib.h"

#include "Scene/Scene.h"

#include "graphics/Lighting/LightManager.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "graphics/Camera/Camera.h"

#include <Input/Input.h>

#include "core/Logger.h"
// preprocessors
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace core {

	Engine::~Engine()
	{
		if (m_imGuiLayer) {
			m_imGuiLayer->Shutdown();
		}
	}

	bool Engine::run()
	{
		OpenGLSetUpResources();

		const auto window = m_window->getGLFWwindow();

		if (!window) {
			Logger::error("[Engine::run] GLFW window returning nullptr!");
			return false;
		}

		Logger::info("Starting engine...");

		// engine life loop
		while (!glfwWindowShouldClose(window) && !m_RequestShutdown)
		{
			OpenGLRenderStuff();

			cameraInput->processInput(window);

			Input::update();

			m_imGuiLayer->BeginFrame();

			rendererManager->draw(scene, cameraManager->getViewMatrix(), cameraManager->getProjectionMatrix());

			m_RequestShutdown = m_imGuiLayer->imGuiImplementations(scene);

			m_imGuiLayer->EndFrame();

			glfwRenderEventStuff();
		}

		std::cout << "iwbs!" << "\n";
		return true;
	}

	bool Engine::initResources()
	{
		// Load materials
		if (!m_MaterialLibraryPtr->createMaterials(MATERIALS_JSON_PATH, *textureManager)) {
			Logger::error("[Engine::initResources] loadMaterialFromJSON function is not working correctly!");
			return false;
		}

		// Load shaders
		if (!shaderManager->loadAllShaders()) {
			Logger::error("[Engine::initResources] loadAllShaders FAILED!");
			return false;
		}

		// load scene object and others.
		if (!scene->SetUpResources(m_MaterialLibraryPtr)) {
			Logger::error("[Engine::initResources] scene->SetUpResources FAILED!");
			return false;
		}

		m_imGuiLayer->Init(m_window->getGLFWwindow());

		// i can back later!!
		// scene->initGrid(renderData);

		Logger::info("Engine initResources successful!");
		return true;
	}

	void Engine::initPointerObjects()
	{
		try {
			initWindow();
			initCallBack();
			initShader();
			initTexture();
			initMaterial();
			initCamera();
			initTransform();
			initRender();
			initMesh();
			initImGui();
			initScene();
			initGrid();
			initLighting();

			Logger::info("Engine initPointerObjects successful!");
		}
		catch (const std::exception& e) {
			Logger::error("Engine::initPointerObjects FAILED! Error: " + std::string(e.what()));
			throw;
		}
	}

	void Engine::initWindow()
	{
		m_window = std::make_unique<Window>();

		DEBUG_PTR(m_window);

		if (!m_window) {
			Logger::error("Engine::initWindow FAILED because of m_window nullptr!");
			throw std::runtime_error("Failed to initialize window FAILED because of m_window nullptr!");
		}
		if (!m_window->initResources()) {
			Logger::error("Engine::initWindow FAILED because of initResources!");
			throw std::runtime_error("Failed to initialize window FAILED because of initResources!");
		}
	}

	void Engine::initCallBack() noexcept
	{
		CallBack::initResources(m_window->getGLFWwindow());
	}

	void Engine::initShader()
	{
		shaderManager = std::make_unique<SHADER::ShaderManager>();

		if (!shaderManager) {
			Logger::warn("[Engine::initShader] shaderManager object is nullptr");
			throw std::runtime_error("Failed to initialize shader manager!");
		}
	}

	void Engine::initTexture()
	{
		textureManager = std::make_shared<TEXTURE::TextureManager>();

		DEBUG_PTR(textureManager);

		if (!textureManager) {
			Logger::warn("[Engine::initTexture] textureManager is nullptr!");
			throw std::runtime_error("Failed to initialize texture!");
		}
	}

	void Engine::initMaterial()
	{
		using namespace MATERIAL;

		m_MaterialLibraryPtr = std::make_shared<MaterialLibrary>();

		DEBUG_PTR(m_MaterialLibraryPtr);

		if (!m_MaterialLibraryPtr) {
			Logger::warn("[Engine::initMaterial] m_MaterialLibraryPtr is nullptr!");
			throw std::runtime_error("Failed to initialize material library!");
		}
	}

	void Engine::initCamera()
	{
		cameraManager = std::make_shared<CAMERA::Camera>();

		DEBUG_PTR(cameraManager);

		if (!cameraManager) {
			Logger::error("[Engine::initCamera] cameraManager is nullptr!");
			throw std::runtime_error("Failed to initialize camera!");
		}

		cameraInput = std::make_unique<CAMERA::CameraInputSystem>(cameraManager.get(), dataInputContext);

		DEBUG_PTR(cameraInput);

		if (!cameraInput) {
			Logger::warn("[Engine::initCamera] cameraInput is nullptr!");
			throw std::runtime_error("Failed to initialize cameraInput!");
		}
	}

	void Engine::initTransform()
	{
		transformationManager = std::make_shared<GLgraphics::Transformations>();

		DEBUG_PTR(transformationManager);

		if (!transformationManager) {
			Logger::warn("[Engine::initTransform] transformationManager is nullptr!");
			throw std::runtime_error("Failed to initialize transformationManager!");
		}

		transformInput = std::make_unique<TRANSFORM_INPUT::TransformInputSystem>(transformationManager.get(), dataInputContext);

		if (!transformInput) {
			Logger::warn("[Engine::initTransform] transformInput is nullptr!");
			throw std::runtime_error("Failed to initialize transformInput!");
		}
	}

	void Engine::initRender()
	{
		renderData = std::make_shared<GLgraphics::RenderData>(shaderManager, cameraManager, textureManager, m_MaterialLibraryPtr);

		if (!renderData) {
			Logger::warn("[Engine::initRender] renderData is nullptr!");
			throw std::runtime_error("Failed to initialize renderData!");
		}

		rendererManager = std::make_unique<GLgraphics::Renderer>(renderData);

		if (!rendererManager) {
			Logger::warn("[Engine::initRender] rendererManager is nullptr!");
			throw std::runtime_error("Failed to initialize rendererManager!");
		}
	}

	void Engine::initMesh()
	{
		sceneObjectFactory = std::make_unique<SCENE::SceneObjectFactory>(renderData);

		DEBUG_PTR(sceneObjectFactory);

		if (!sceneObjectFactory) {
			Logger::error("Failed to initialize SceneObjectFactory");
			throw std::runtime_error("Failed to initialize SceneObjectFactory!");
		}

		meshData3D = sceneObjectFactory->getMeshData();
	}

	void Engine::initImGui()
	{
		m_imGuiLayer = std::make_unique<ENGINE::UI::ImGuiLayer>(renderData, m_window->getGLFWwindow());

		if (!m_imGuiLayer) {
			Logger::warn("[Engine::initImGui] m_imGuiLayer object is nullptr");
			throw std::runtime_error("Failed to initialize ImGuiLayer!");
		}
	}

	void Engine::initScene()
	{
		scene = std::make_shared<SCENE::Scene>(meshData3D, dataInputContext);

		if (!scene) {
			Logger::warn("[Engine::initScene] scene is nullptr!");
			throw std::runtime_error("Failed to initialize scene!");
		}

		scene->setSceneObjectFactoryPointer(sceneObjectFactory.get());

		sceneObjectFactory->setScene(scene);
	}

	void Engine::initGrid()
	{
		// Init Grid
		m_gridSystem = std::make_unique<GRID::GridSystem>();

		if (!m_gridSystem) {
			Logger::warn("[Engine::initGrid] m_gridSystem returning nullptr");
			throw std::runtime_error("Failed to initialize GridSystem!");
		}

		if (!m_gridSystem->initialize(cameraManager)) {
			Logger::warn("[Engine::initGrid] m_gridSystem initializing failed!");
			throw std::runtime_error("Failed to initialize GridSystem!");
		}

		renderData->setGridRenderer(m_gridSystem->getRenderer());
		scene->setOwnershipGridSystemToScene(m_gridSystem);

		// before we use grid renderer we have to set a shader interface(like grid shader) to gridRenderer!
		// so create a shader interface for grid shader, and send to the gridRenderer, because needed for drawing!
	}

	void Engine::initLighting()
	{
		lightManager = std::make_shared<LIGHTING::LightManager>();

		if (!lightManager) {
			Logger::warn("[Engine::initLighting] lightManager is nullptr!");
			throw std::runtime_error("[Engine::initLighting] lightManager is nullptr!");
		}

		renderData->setLightManager(lightManager);
	}

	void Engine::OpenGLSetUpResources() noexcept
	{
		glEnable(GL_DEPTH_TEST);
	}

	void Engine::OpenGLRenderStuff() noexcept
	{
		// Screen Color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Engine::glfwRenderEventStuff() const noexcept {
		// GLFW stuff
		glfwPollEvents();
		glfwSwapBuffers(m_window->getGLFWwindow());
	}
}