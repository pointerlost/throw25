project(engine)

# define static engine library
add_library(engine STATIC
        include/Entity.h
        include/ComponentStorage.h
        include/World.h
        include/graphics/Mesh/MeshComponent.h
        src/graphics/Mesh/MeshRenderSystem.cpp
        include/graphics/Mesh/MeshRenderSystem.h
        src/ComponentStorage.cpp
        include/graphics/Transformations/TransformComponent.h
        src/World.cpp
        src/Math/Math.cpp
        include/Math/Math.h
        include/graphics/Lighting/LightComponent.h
        include/graphics/Material/MaterialComponent.h
        include/graphics/Camera/CameraComponent.h
)

# define header and source paths
target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(engine PUBLIC cxx_std_23)

target_sources(engine PRIVATE
    # Core
    src/core/Engine.cpp
    src/core/Window.cpp
    src/core/File.cpp

    include/core/Engine.h
    include/core/Window.h
    include/core/File.h

    include/core/Logger.h

    # Input
    src/Input/Input.cpp
    src/Input/InputContext.cpp
    src/Input/InputComponent.cpp
    src/Input/InputComponentFactory.cpp

    include/Input/Input.h
    include/Input/InputContext.h
    include/Input/InputComponent.h
    include/Input/InputComponentFactory.h

    # Callback
    src/CallBack/CallBack.cpp
    include/CallBack/CallBack.h

    # Meshes
    src/graphics/Mesh/Mesh3D.cpp
    src/graphics/Mesh/MeshFactory.cpp
    src/graphics/Mesh/MeshData3D.cpp
    src/graphics/Mesh/MeshRenderSystem.cpp

    include/graphics/Mesh/Mesh3D.h
    include/graphics/Mesh/MeshInterface.h
    include/graphics/Mesh/MeshFactory.h
    include/graphics/Mesh/MeshData3D.h
    include/graphics/Mesh/MeshRenderSystem.h

    # Shader System
    src/graphics/Shaders/ShaderProgram.cpp
    src/graphics/Shaders/ShaderManager.cpp
    src/graphics/Shaders/BasicShader.cpp
    src/graphics/Shaders/GridShader.cpp

    include/graphics/Shaders/ShaderInterface.h
    include/graphics/Shaders/ShaderProgram.h
    include/graphics/Shaders/ShaderManager.h
    include/graphics/Shaders/BasicShader.h
    include/graphics/Shaders/GridShader.h

    # Camera
    src/graphics/Camera/Camera.cpp
    src/graphics/Camera/CameraController.cpp

    include/graphics/Camera/Camera.h
    include/graphics/Camera/CameraController.h

    # Grid
    src/graphics/Grid/GridData.cpp
    src/graphics/Grid/GridRenderer.cpp
    src/graphics/Grid/GridSystem.cpp

    include/graphics/Grid/GridData.h
    include/graphics/Grid/GridRenderer.h
    include/graphics/Grid/GridSystem.h

    # Renderer
    src/graphics/Renderer/RenderData.cpp
    src/graphics/Renderer/Renderer.cpp

    include/graphics/Renderer/RenderData.h
    include/graphics/Renderer/Renderer.h

    # Lighting
    src/graphics/Lighting/Light.cpp
    src/graphics/Lighting/LightData.cpp
    src/graphics/Lighting/LightManager.cpp

    include/graphics/Lighting/Light.h
    include/graphics/Lighting/LightData.h
    include/graphics/Lighting/LightManager.h

    # Scene
    src/Scene/Scene.cpp
    src/Scene/SceneObject.cpp
    src/Scene/SceneObjectFactory.cpp

    include/Scene/Scene.h
    include/Scene/SceneObject.h
    include/Scene/SceneObjectFactory.h

    # Material & Textures
    src/graphics/Material/MaterialLib.cpp
    include/graphics/Material/MaterialLib.h

    src/graphics/Textures/Textures.cpp
    src/graphics/Textures/stb_image.cpp
    include/graphics/Textures/Textures.h
    include/graphics/Textures/stb_image.h

    # ImGui Layer
    third_party/ImGuizmo/ImGuizmo.cpp

    src/ImGui/ImGuiLayer.cpp
    src/ImGui/ImGuiScoped.cpp
    src/ImGui/ImGuiObjectState.cpp
    src/ImGui/TransformGizmo.cpp

    include/ImGui/ImGuiLayer.h
    include/ImGui/ImGuiScoped.h
    include/ImGui/ImGuiObjectState.h
    include/ImGui/TransformGizmo.h

    # Transformations
    src/graphics/Transformations/Transformations.cpp

    include/graphics/Transformations/Transformations.h
    include/graphics/Transformations/TransformComponent.h

    src/World.cpp
    include/World.h

    src/Math/Math.cpp
    include/Math/Math.h

    include/Math/RayMath.h
)

add_custom_target(copy_asset_dir
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/assets"
        COMMENT "Copying assets folder completely into build"
)

add_dependencies(engine copy_asset_dir)

add_custom_target(copy_shader
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "${CMAKE_CURRENT_BINARY_DIR}/shaders"
        COMMENT "Copying shaders folder to build dir"
)

add_dependencies(engine copy_shader)

set(SHADERS_CONFIG_PATH "${CMAKE_CURRENT_BINARY_DIR}/shaders/config/shaders.json")

# Set SHADERS_DIR, ASSETS_DIR and SHADERS_CONFIG_PATH
set(SHADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
set(ASSETS_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")
set(SHADERS_CONFIG_PATH "${CMAKE_CURRENT_BINARY_DIR}/shaders/config/shaders.json")

# Configure Config.h from Config.h.in
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core/Config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/core/Config.h
    @ONLY
)

# Find packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Link libraries
target_link_libraries(engine
    PUBLIC
    glfw
    glad::glad
    glm::glm
    nlohmann_json::nlohmann_json
    imgui::imgui
)
