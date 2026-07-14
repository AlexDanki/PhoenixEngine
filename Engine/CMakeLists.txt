add_library(MunitaEngine STATIC)

target_sources(MunitaEngine PRIVATE  
"src/Core/Window.cpp" "src/Graphics/Renderer.cpp" "include/Graphics/Mesh.h" "src/Graphics/Mesh.cpp" "src/Core/Transform.cpp"  "src/Core/GameObject.cpp" "include/Core/Scene.h" "src/Core/Scene.cpp" "include/Components/Component.h" "src/Components/Component.cpp" "include/Components/MeshComponent.h" "src/Components/MeshComponent.cpp" "include/Core/App.h" "src/Core/App.cpp" "include/Graphics/Camera.h" "src/Graphics/Camera.cpp" "include/Graphics/Vertex.h" "include/Graphics/DirectionalLight.h" "include/Graphics/AmbienteLight.h" "include/Graphics/RenderContext.h" "include/Graphics/Texture.h" "src/Graphics/Texture.cpp" "include/Graphics/Material.h" "src/Graphics/Material.cpp" "include/Graphics/Shader.h" "src/Graphics/Shader.cpp" "include/Core/AssetManager.h" "src/Core/AssetManager.cpp" "include/Serializers/SceneSerializer.h" "src/Serializers/SceneSerializer.cpp"   "include/Graphics/MeshAsset.h" "../ThirdParty/ImGui/imgui.cpp"
    "../ThirdParty/ImGui/imgui_draw.cpp"
    "../ThirdParty/ImGui/imgui_tables.cpp"
    "../ThirdParty/ImGui/imgui_widgets.cpp"

    "../ThirdParty/ImGui/imgui_impl_glfw.cpp"
    "../ThirdParty/ImGui/imgui_impl_opengl3.cpp"

    "../ThirdParty/ImGui/imgui_demo.cpp" "src/Graphics/MeshAsset.cpp" "include/Components/BoxCollider.h" "src/Components/BoxCollider.cpp" "include/Components/RigidbodyComponent.h" "src/Components/RigidbodyComponent.cpp" "include/Editor/EditorLayer.h" "src/Editor/EditorLayer.cpp" "include/Components/ScriptComponent.h"  "include/Game/RotatorScript.h" "include/Components/CameraComponent.h" "src/Components/CameraComponent.cpp" "include/Core/Input.h" "src/Core/Input.cpp" "include/Utils/Log.h" "include/Game/PaddleController.h" "src/Game/PaddleController.cpp" "include/Core/ScriptRegistry.h" "src/Core/ScriptRegistry.cpp" "include/Physics/PhysicsSystem.h" "src/Physics/PhysicsSystem.cpp" "include/Game/BallController.h" "src/Game/BallController.cpp" "include/Game/GameManager.h" "include/Utils/Random.h" "src/Utils/Random.cpp" "src/Game/GameManager.cpp" "include/Game/EnemyPaddleController.h" "src/Game/EnemyPaddleController.cpp" "include/Graphics/Font.h" "src/Graphics/Font.cpp" "include/Graphics/FontLibrary.h" "src/Graphics/FontLibrary.cpp" "src/Graphics/WorldTextRenderComponent.cpp" "include/Serializers/SerializeUtils.h" "src/Serializers/SerializeUtils.cpp" "include/Components/ComponentFactory.h" "src/Components/ComponentFactory.cpp" "include/Serializers/SerializeContext.h" "include/Graphics/Fog.h" "src/Core/EngineTime.cpp" "include/Core/EngineTime.h" "include/Graphics/Framebuffer.h" "src/Graphics/Framebuffer.cpp")


target_include_directories(MunitaEngine
PUBLIC	include 
"../ThirdParty/ImGui"
${CMAKE_SOURCE_DIR}/Engine/include
${CMAKE_SOURCE_DIR}/Engine/include/Core
${CMAKE_SOURCE_DIR}/Engine/include/Game
${CMAKE_SOURCE_DIR}/Engine/include/Utils
${CMAKE_SOURCE_DIR}/Engine/include/Editor
${CMAKE_SOURCE_DIR}/Engine/include/Physics
${CMAKE_SOURCE_DIR}/Engine/include/Graphics
${CMAKE_SOURCE_DIR}/Engine/include/Serializers
${CMAKE_SOURCE_DIR}/Engine/include/Components
${CMAKE_SOURCE_DIR}/ThirdParty/ImGui
${CMAKE_SOURCE_DIR}/ThirdParty/ImGui/backends
)

find_package(glfw3 CONFIG REQUIRED)	
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)

target_link_libraries(MunitaEngine PUBLIC glfw glad::glad glm::glm assimp::assimp freetype)
