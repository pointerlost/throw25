#include "Scene/SceneObjectFactory.h"

#include "graphics/Material/MaterialLib.h"
#include "Scene/Scene.h"
#include "Scene/SceneObject.h"
#include "graphics/Lighting/LightData.h"
#include "graphics/Lighting/Light.h"
#include "graphics/Mesh/GLMeshCube3D.h"
#include "graphics/Mesh/GLMeshSphere3D.h"
#include "graphics/Mesh/GLMeshFactory.h"
#include "graphics/Mesh/GLMesh3D.h"
#include "graphics/Renderer/RenderData.h"
#include <Input/InputComponent.h>
#include <Input/InputComponentFactory.h>
#include "graphics/GLTransformations/Transformations.h"
#include <core/Logger.h>
#include "graphics/Lighting/LightManager.h"
#include "graphics/Shaders/BasicShader.h"


constexpr auto BASIC_SHADER = "basic";

namespace SCENE
{

    struct SceneObjectFactory::Impl
    {
        std::unique_ptr<GLgraphics::MeshFactory> meshFactory;
        std::shared_ptr<GLgraphics::MeshData3D> meshData;
        std::unique_ptr<GLgraphics::Mesh> meshManager;

        void initBaseMeshes() const {
            auto addMesh = [&](const std::string& name) {
                auto [vertices, indices] = meshFactory->createMeshObject(name);
                meshData->AddMesh3DToMeshData(name, vertices, indices);
                };
            addMesh("cube");
            addMesh("sphere");
            addMesh("triangle");
            addMesh("square");
            addMesh("circle");
        }
    };

	SceneObjectFactory::SceneObjectFactory(const std::shared_ptr<GLgraphics::RenderData>& renderData)
        :m_pImpl(std::make_unique<Impl>()), m_renderData(renderData)
	{
		m_pImpl->meshFactory = std::make_unique<GLgraphics::MeshFactory>();
		m_pImpl->meshData = std::make_shared<GLgraphics::MeshData3D>();
		m_pImpl->initBaseMeshes();
		m_pImpl->meshManager = std::make_unique<GLgraphics::Mesh>(m_pImpl->meshData);
	}

    SceneObjectFactory::~SceneObjectFactory() = default; // Impl is complete here

    std::shared_ptr<GLgraphics::MeshData3D> SceneObjectFactory::getMeshData() const
    {
        return m_pImpl->meshData;
    }

    void SceneObjectFactory::initBaseMeshes() const {
		// This function initializes the base meshes used in the scene.
        auto addMesh = [&](const std::string& name) {
            auto [vertices, indices] = m_pImpl->meshFactory->createMeshObject(name);
            m_pImpl->meshData->AddMesh3DToMeshData(name, vertices, indices);
        };

		// Add predefined meshes to the mesh data
        addMesh("cube");
        addMesh("sphere");
        addMesh("triangle");
        addMesh("square");
        addMesh("circle");
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::createCube(const glm::vec3& pos, const std::string& materialName, bool visualLightObj) const
    {
        // Create cube as a scene object
        auto cube = std::make_shared<SceneObject>(
            std::make_shared<GLgraphics::MeshCube3D>(m_pImpl->meshData,
                m_pImpl->meshData->getObjectInfo("cube")), materialName
        );
        cube->getTransform()->setPosition(pos);

        if (!cube) {
            Logger::warn("[SceneObjectFactory::createCube] cube is nullptr");
            return {};
        }

  //       // Create an input component for the cube
  //       const auto inputComponent = Input::InputComponentFactory::createObjectComponent(
  //           Input::InputType::CubeInputComponent,
  //           cube->getTransform()
		// );
  //
  //       if (inputComponent) {
		// 	cube->setInputComponent(inputComponent);
  //       } else {
  //           Logger::warn("[SceneObjectFactory::createCube] Input component creation failed.");
		// }

        cube->setShaderInterface(m_renderData->getShaderInterface(BASIC_SHADER));
        cube->setName(generateName("cube"));
        cube->initializeMaterial(m_renderData->getMaterialLib());
        cube->getTransform()->setPosition(glm::vec3(0.0));
        cube->getTransform()->setScale(glm::vec3{ 7.5f, 7.5f, 7.5f });

        if (!visualLightObj)
            m_scene->createObjectProperties(cube);

        return cube;
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::createSphere(const glm::vec3& pos, const std::string& materialName, bool visualLightObj) const
    {
		// Create a sphere object with the specified material and position
        auto sphere = std::make_shared<SceneObject>(
            std::make_shared<GLgraphics::MeshSphere3D>(m_pImpl->meshData,
                m_pImpl->meshData->getObjectInfo("sphere")), materialName
        );
        sphere->getTransform()->setPosition(pos);

        if (!sphere) {
            Logger::warn("[SceneObjectFactory::createSphere] sphere is nullptr");
            return {};
        }

        // // Create an input component for the cube
        // const auto inputComponent = Input::InputComponentFactory::createObjectComponent(
        //     Input::InputType::SphereInputComponent,
        //     sphere->getTransform()
        // );
        //
        // if (inputComponent) {
        //     sphere->setInputComponent(inputComponent);
        // }
        // else {
        //     Logger::warn("[SceneObjectFactory::createSphere] Input component creation failed.");
        // }

        sphere->setShaderInterface(m_renderData->getShaderInterface(BASIC_SHADER));
        sphere->setName(generateName("sphere"));
        sphere->initializeMaterial(m_renderData->getMaterialLib());
        sphere->getTransform()->setPosition(glm::vec3(0.0));
        sphere->getTransform()->setScale(glm::vec3{ 3.5f, 3.5f, 3.5f });

        if (!visualLightObj)
            m_scene->createObjectProperties(sphere);

        return sphere;
    }

    bool SceneObjectFactory::createPointLight(const std::string& materialName,
        const glm::vec3& position, LIGHTING::LightMobility mobility) const
    {
		// Create visual representation (sphere)
        const auto lightVisualObject = createSphere(position, materialName, true);

        if (!lightVisualObject) {
            Logger::error("[SceneObjectFactory::createPointLight] Failed to create lightVisualObject.");
            return false;
		}

        lightVisualObject->setShaderInterface(m_renderData->getShaderInterface(BASIC_SHADER));
        lightVisualObject->setName(generateName("point"));

        auto lightData = std::make_shared<LIGHTING::LightData>(position);

        auto light = std::make_shared<LIGHTING::Light>(lightData);
        light->setLightType(LIGHTING::LightType::Point);
        light->setVisualObject(lightVisualObject);
        light->setMobility(mobility);

        m_scene->createObjectProperties(lightVisualObject);

        switch (mobility) {
            case LIGHTING::LightMobility::Static:
                m_renderData->getLightManager()->addStaticLight(light);
                break;
            case LIGHTING::LightMobility::Stationary:
                m_renderData->getLightManager()->addStationaryLight(light);
                break;
            case LIGHTING::LightMobility::Dynamic:
                m_renderData->getLightManager()->addDynamicLight(light);
                break;

            default:
                m_renderData->getLightManager()->addStationaryLight(light);
                break;
        }

        // // we have to create a new input component for the light object overwriting the existing one
        // const auto inputComponent = Input::InputComponentFactory::createLightComponent(
        //     Input::InputType::LightInputComponent,
        //     lightVisualObject->getTransform(),
        //     light,
        //     mobility
        // );
        //
        // inputComponent ? lightVisualObject->setInputComponent(inputComponent)
        // : Logger::warn("[SceneObjectFactory::createPointLight] Input component creation failed.");

        return true;
    }

    bool SceneObjectFactory::createDirectionalLight(const std::string& materialName,
        const glm::vec3& position, LIGHTING::LightMobility mobility) const
    {
        // Create visual representation (sphere)
        const auto lightVisualObject = createSphere(position, materialName, true);

        if (!lightVisualObject) {
            Logger::error("[SceneObjectFactory::createDirectionalLight] Failed to create lightVisualObject.");
            return false;
		}

        lightVisualObject->setShaderInterface(m_renderData->getShaderInterface(BASIC_SHADER));
        lightVisualObject->setName(generateName("directional"));

        auto lightData = std::make_shared<LIGHTING::LightData>(position);

        auto light = std::make_shared<LIGHTING::Light>(lightData);
        light->setLightType(LIGHTING::LightType::Directional);
        light->setVisualObject(lightVisualObject);
        light->setMobility(mobility);

        m_scene->createObjectProperties(lightVisualObject);

        switch (mobility) {
            case LIGHTING::LightMobility::Static:
                m_renderData->getLightManager()->addStaticLight(light);
                break;
            case LIGHTING::LightMobility::Stationary:
                m_renderData->getLightManager()->addStationaryLight(light);
                break;
            case LIGHTING::LightMobility::Dynamic:
                m_renderData->getLightManager()->addDynamicLight(light);
                break;

            default:
                m_renderData->getLightManager()->addStationaryLight(light);
                break;
        }

        // we have to create a new input component for the light object overwriting the existing one
        // because already created to a light visual object like a sphere!
        // const auto inputComponent = Input::InputComponentFactory::createLightComponent(
        //     Input::InputType::LightInputComponent,
        //     lightVisualObject->getTransform(),
        //     light,
        //     mobility
        // );
        //
        // inputComponent ? lightVisualObject->setInputComponent(inputComponent)
        // : Logger::warn("[SceneObjectFactory::createPointLight] Input component creation failed.");

        return true;
    }

    bool SceneObjectFactory::createSpotLight(const std::string& materialName,
        const glm::vec3& position, LIGHTING::LightMobility mobility) const
    {
		// Create visual representation (sphere)
        const auto lightVisualObject = createSphere(position, materialName, true);

        if (!lightVisualObject) {
            Logger::error("[SceneObjectFactory::createSpotLight] Failed to create lightVisualObject.");
            return false;
        }

        lightVisualObject->setShaderInterface(m_renderData->getShaderInterface(BASIC_SHADER));
        lightVisualObject->setName(generateName("spot"));

        auto lightData = std::make_shared<LIGHTING::LightData>(position);

        auto light = std::make_shared<LIGHTING::Light>(lightData);
        light->setLightType(LIGHTING::LightType::Spot);
        light->setVisualObject(lightVisualObject);
        light->setMobility(mobility);

        m_scene->createObjectProperties(lightVisualObject);

        switch (mobility) {
            case LIGHTING::LightMobility::Static:
                m_renderData->getLightManager()->addStaticLight(light);
                break;
            case LIGHTING::LightMobility::Stationary:
                m_renderData->getLightManager()->addStationaryLight(light);
                break;
            case LIGHTING::LightMobility::Dynamic:
                m_renderData->getLightManager()->addDynamicLight(light);
                break;

            default:
                m_renderData->getLightManager()->addStationaryLight(light);
                break;
        }

        // we have to create a new input component for the light object overwriting the existing one
        // const auto inputComponent = Input::InputComponentFactory::createLightComponent(
        //     Input::InputType::LightInputComponent,
        //     lightVisualObject->getTransform(),
        //     light,
        //     mobility
        // );
        //
        // inputComponent ? lightVisualObject->setInputComponent(inputComponent)
        // : Logger::warn("[SceneObjectFactory::createPointLight] Input component creation failed.");

		return true;
    }

    std::string SceneObjectFactory::generateName(const std::string &type)
    {
        static std::unordered_map<std::string, uint32_t> typeCounters;
        uint32_t& counter = typeCounters[type];
        ++counter;
        return type + "_" + std::to_string(counter);
    }
}
