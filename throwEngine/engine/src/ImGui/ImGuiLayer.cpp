#include "ImGui/ImGuiLayer.h"

#include "ImGui/ImGuiScoped.h"

#include <imgui_impl_glfw.h>

#include <imgui_impl_opengl3.h>

#include "graphics/Renderer/RenderData.h"

#include "graphics/Lighting/Light.h"

#include "graphics/Lighting/LightManager.h"

#include <graphics/GLTransformations/Transformations.h>

#include "Scene/Scene.h"

#include "Scene/SceneObject.h"

#include "graphics/Material/MaterialLib.h"

#include <graphics/Shaders/ShaderInterface.h>

#include "core/Config.h"

#include <core/File.h>

#include "core/Logger.h"
#include "Scene/SceneObjectFactory.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace ENGINE::UI
{
	ImGuiLayer::ImGuiLayer(std::shared_ptr<GLgraphics::RenderData> renderData, GLFWwindow* window)
		:m_renderData(renderData), m_Window(window) {}
	
	void ImGuiLayer::Init(GLFWwindow* window)
	{
		m_Window = window;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsClassic();

		// set the ImGui font style and text size
		initFont(io);

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 440");	// according to shader version
	}

	void ImGuiLayer::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::drawSceneEditorUI(const std::shared_ptr<SCENE::Scene> &scene)
	{
		if (!m_showSceneEditorUI) return;

		const auto sceneObjectFactory = scene->getSceneObjectFactory();

		const auto lightCount = m_renderData->getLightManager()->getActiveLightCount();

		const auto sceneObjects = scene->getSceneObjectVec();

		ImGuiIO& io = ImGui::GetIO();

		if (const ImGuiScopedWindow addButton("Add Object"); addButton)
		{
			if (ImGui::Button("Cube")) {
				(void)sceneObjectFactory->createCube();
			}
			if (ImGui::Button("Sphere")) {
				(void)sceneObjectFactory->createSphere();
			}
			if (ImGui::Button("Point Light")) {
				(void)sceneObjectFactory->createPointLight();
			}
			if (ImGui::Button("Directional light")) {
				(void)sceneObjectFactory->createDirectionalLight();
			}
			if (ImGui::Button("Spot Light")) {
				(void)sceneObjectFactory->createSpotLight();
			}
		}

		if (const ImGuiScopedWindow deleteButton("Delete Object"); deleteButton) {
			if (ImGui::Button("Delete Object")) {
				bool anyDeleted = false;

				for (const auto & sceneObject : sceneObjects) {
					const std::string& name = sceneObject->getName();
					if (m_objectUIStates[name].isOpen) {
						scene->markObjectsToDelete(name);
						anyDeleted = true;
					}
				}
				if (!anyDeleted) {
					Logger::warn("[ImGuiLayer::drawSceneEditorUI] No objects were selected for deletion.");
				}
			}
		}
	}

	void ImGuiLayer::initFont(const ImGuiIO& io)
	{
		const auto& file = core::File::get();

		m_fontPath = std::string(ASSETS_DIR) + "/fonts/RobotoSlab.ttf";

		if (file.exists(m_fontPath)) {
			io.Fonts->AddFontFromFileTTF(m_fontPath.c_str(), 20.0f);
		}
		else {
			Logger::error("[ImGuiLayer::Init::initFont] Font file not found at path! so Default path assigned!");
			io.Fonts->AddFontDefault();
		}
	}

	bool ImGuiLayer::imGuiImplementations(const std::shared_ptr<SCENE::Scene>& scene)
	{
		generalMenuForPanel();

		sceneObjectSelectionPanel(scene->getSceneObjectVec());

		implForRenderObjects(scene->getSceneObjectVec());

		lightingPanelWindow();

		drawSceneEditorUI(scene);

		return g_RequestShutdown;
	}

	void ImGuiLayer::setLightObjectProperties()
	{
		const std::string panelTitle = "Lighting Panel";

		// we don't have to call ImGui::End() because we are using destructor of ImGuiScopedWindow for that! (RAII pattern)
		ImGuiScopedWindow lightingPanelWindow(panelTitle.c_str(), nullptr, ImGuiWindowFlags_None);

		auto& io = ImGui::GetIO();

		// const auto lights = m_renderData->getLightManager()->getLightsByVec();

		const uint32_t lightSize = m_renderData->getLightManager()->getActiveLightCount();

		if (lightSize <= 0) return;

		// if (lights.empty()) {
		// 	Logger::warn("lights vector are empty!");
		// 	ImGui::Text("No lights available!");
		// 	return;
		// }

		static int selectedLightIndex = 0;

		ImGui::SliderInt("Change Light Object", &selectedLightIndex, 0, static_cast<int>(lightSize) - 1);

		if (selectedLightIndex >= lightSize) return;

		/* I have to add "OBJECT DETECTION SYSTEM" to update here! */
		/*

		// find current light
		const auto staticLights = m_renderData->getLightManager()->getStaticLights();
		for (auto obj : staticLights) {
			if (obj.lock() && obj.lock().get() == )
		}

		// current light
		const auto& currentLight = lights[selectedLightIndex];

		// we have to create a temporary variable to hold the light type
		LIGHTING::LightType currentType = currentLight->getType();
		int typeInt = static_cast<int>(currentType);

		// set light type
		// 0 = Directional, 1 = Point, 2 = Spot
		if (ImGui::SliderInt("SetLightType", &typeInt, 0, 2)) {
			currentType = static_cast<LIGHTING::LightType>(typeInt);
			currentLight->setLightType(currentType);
		}

		ImGui::ColorEdit3("Set Ambient of Light Color",  glm::value_ptr(currentLight->getLightData()->getAmbient())  );
		ImGui::ColorEdit3("Set Diffuse of Light Color",  glm::value_ptr(currentLight->getLightData()->getDiffuse())  );
		ImGui::ColorEdit3("Set Specular of Light Color", glm::value_ptr(currentLight->getLightData()->getSpecular()) );

		currentLight->update(currentLight->getVisualObject());

		*/
	}

	void ImGuiLayer::implForRenderObjects(std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects)
	{
		for (auto& obj : sceneObjects)
		{
			const std::string& name = obj->getName();
			auto& state = m_objectUIStates[name];

			if (!state.isOpen) continue;

			ImGui::SetNextWindowSize(ImVec2(400, 300));

			if (const ImGuiScopedWindow objectWindow((name + std::string(" | Object Properties")).c_str(), &state.isOpen); objectWindow)
			{
				showSelectedObjectProperties(obj);
			}
		}
	}

	void ImGuiLayer::sceneObjectSelectionPanel(const std::vector<std::shared_ptr<SCENE::SceneObject>>& sceneObjects)
	{
		if (!m_showObjectListWindow) return;

		const uint32_t objectsSize = sceneObjects.size();

		ImGui::SetNextWindowSize(ImVec2(static_cast<float>(objectsSize * 20), static_cast<float>(objectsSize * 20)), ImGuiCond_Once);

		// ImGuiScopedWindow is a RAII wrapper for ImGui::Begin() and ImGui::End()
		if (const ImGuiScopedWindow objectList("Scene Objects", &m_showObjectListWindow); objectList)
		{
			for (auto& obj : sceneObjects)
			{
				const std::string name = obj->getName();
				auto& state = m_objectUIStates[name];

				if (ImGui::Selectable(name.c_str()))
				{
					state.isOpen = !state.isOpen;
				}
			}
		}
	}

	void ImGuiLayer::lightingPanelWindow()
	{
		if (!m_showLightSourceObjects) return;

		ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);

		if (const ImGuiScopedWindow lightingPanel("Lighting Panel", &m_showLightSourceObjects); lightingPanel)
		{
			setLightObjectProperties();
		}
	}

	void ImGuiLayer::showTransformProperties(std::shared_ptr<SCENE::SceneObject>& object, UIObjectState& state)
	{
		if (const ImGuiScopedMenu transformProperties("TRANSFORMATIONS"); transformProperties) {
			if (ImGui::IsItemHovered())
			{
				state.isTransformOpen = !state.isTransformOpen;
			}

			if (state.isTransformOpen)
			{
				setSceneObjectTransform(object);
			}
		}
	}

	void ImGuiLayer::showMaterialProperties(std::shared_ptr<SCENE::SceneObject>& object, UIObjectState& state)
	{
		if (const ImGuiScopedMenu materialProperties("MATERIAL"); materialProperties) {
			if (ImGui::IsItemHovered())
			{
				state.isMaterialOpen = true;
			}

			if (state.isMaterialOpen)
			{
				setSceneObjectMaterial(object);
			}
		}
	}

	void ImGuiLayer::showSelectedObjectProperties(std::shared_ptr<SCENE::SceneObject>& object)
	{
		auto& state = m_objectUIStates[object->getName()];

		// push style variables to change padding
		ImGuiScopedStyleVar stylePadding(ImGuiStyleVar_FramePadding, ImVec2(15, 12));

		showMaterialProperties(object, state);

		showTransformProperties(object, state);
	}

	void ImGuiLayer::setSceneObjectMaterial(std::shared_ptr<SCENE::SceneObject>& sceneObject)
	{
		const auto object = sceneObject->getShaderInterface();
		const auto material = sceneObject->getMaterialInstance();

		if (!object || !material) {
			std::cout << "has sktrr!\n";
			return;
		}

		auto objectType = object->getType();

		// ambient
		ImGui::SliderFloat("Set ambient  R value", &material->m_ambient.x,  0.05, 1.0);
		ImGui::SliderFloat("Set ambient  G value", &material->m_ambient.y,  0.05, 1.0);
		ImGui::SliderFloat("Set ambient  B value", &material->m_ambient.z,  0.05, 1.0);

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // add some space

		// diffuse
		ImGui::SliderFloat("Set diffuse  R value", &material->m_diffuse.x,  0.0, 1.0);
		ImGui::SliderFloat("Set diffuse  G value", &material->m_diffuse.y,  0.0, 1.0);
		ImGui::SliderFloat("Set diffuse  B value", &material->m_diffuse.z,  0.0, 1.0);

		// set diffuse color using color picker
		ImGui::ColorEdit3("Set diffuse color", glm::value_ptr(material->m_diffuse),
			ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // add some space

		// specular
		ImGui::SliderFloat("Set specular R value", &material->m_specular.x, 0.0, 1.0);
		ImGui::SliderFloat("Set specular G value", &material->m_specular.y, 0.0, 1.0);
		ImGui::SliderFloat("Set specular B value", &material->m_specular.z, 0.0, 1.0);

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // add some space

		// ImGui::Checkbox("Enable diffuse  texture", &material->m_diffuseTexture);
		// ImGui::Checkbox("Enable specular texture", &material->m_specularTexture);

		sceneObject->setMaterialInstance(material);
	}

	void ImGuiLayer::setSceneObjectTransform(const std::shared_ptr<SCENE::SceneObject>& sceneObject)
	{
		ImGui::PushID(sceneObject->getName().c_str()); // Push a unique ID for the object

		const auto &transform = sceneObject->getTransform();
		if (!transform) {
			Logger::error("[ImGuiLayer::setSceneObjectTransform] Transform is nullptr!");
			return;
		}

		auto position = transform->getPosition();
		auto angles   = transform->getEulerAngles();
		auto scale	  = transform->getScale();

		ImGui::Text("Position");
		ImGui::SliderFloat("X##Position", &position.x, -100.0, 100.0);
		ImGui::SliderFloat("Y##Position", &position.y, -100.0, 100.0);
		ImGui::SliderFloat("Z##Position", &position.z, -100.0, 100.0);

		ImGui::Separator();

		ImGui::Text("Rotation (Euler Angles):");
		ImGui::SliderFloat("X##Euler", &angles.x, -180.0, 180.0);
		ImGui::SliderFloat("Y##Euler", &angles.y, -180.0, 180.0);
		ImGui::SliderFloat("Z##Euler", &angles.z, -180.0, 180.0);

		ImGui::Separator();

		ImGui::Text("Scale");
		ImGui::SliderFloat("X##Scale", &scale.x, 0.01, 10.0);
		ImGui::SliderFloat("Y##Scale", &scale.y, 0.01, 10.0);
		ImGui::SliderFloat("Z##Scale", &scale.z, 0.01, 10.0);

		// Update the transform with the new values
		transform->updateAll(angles, position, scale);

		// lightData should set position to a visual object

		ImGui::PopID(); // Pop the ID to avoid conflicts with other objects
	}

	void ImGuiLayer::generalMenuForPanel()
	{
		static bool showNewMenu = false;

		if (const ImGuiScopedMainMenuBar mainMenuBar; mainMenuBar)
		{
			{
				ImGuiScopedMenu engineMenu("ENGINE GUI");
				if (engineMenu)
				{
					if (ImGui::MenuItem("New"))
						showNewMenu = true;

					ImGui::MenuItem("Save", "Ctrl+S");
				}
			} // engineMenu destructor calls EndMenu here

			{

				if (const ImGuiScopedMenu sceneObjectsMenu("SCENE"); sceneObjectsMenu)
				{
					// SceneObjects GUI
					if (ImGui::MenuItem("SceneObjects", nullptr, m_showObjectListWindow))
						m_showObjectListWindow = !m_showObjectListWindow;

					// Engine Shutdown
					if (ImGui::MenuItem("Exit"))
						g_RequestShutdown = true;
				}
			}

			{

				if (const ImGuiScopedMenu lightingMenu("LIGHTING"); lightingMenu)
				{
					if (ImGui::MenuItem("Change Light Types", nullptr, m_showLightSourceObjects))
						m_showLightSourceObjects = !m_showLightSourceObjects;
				}
			}

			{
				if (const ImGuiScopedMenu sceneEditor("EDITOR"); sceneEditor) {
					if (ImGui::MenuItem("Show SceneEditor", nullptr, m_showSceneEditorUI))
						m_showSceneEditorUI = !m_showSceneEditorUI;
				}
			}
		}
	}

	void ImGuiLayer::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	/********************************** ImGuiScopedWindow *************************************/
}