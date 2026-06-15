add_library(PhoenixEngine STATIC)

target_sources(PhoenixEngine PRIVATE  "src/Window.cpp" "src/Renderer.cpp" "include/Mesh.h" "src/Mesh.cpp" "src/Transform.cpp"  "src/GameObject.cpp" "include/Scene.h" "src/Scene.cpp" "include/Component.h" "src/Component.cpp" "include/MeshComponent.h" "src/MeshComponent.cpp" "include/App.h" "src/App.cpp" "include/Camera.h" "src/Camera.cpp" "include/Vertex.h" "include/DirectionalLight.h" "include/AmbienteLight.h" "include/RenderContext.h" "include/Texture.h" "src/Texture.cpp" "include/Material.h" "src/Material.cpp" "include/Shader.h" "src/Shader.cpp" "include/AssetManager.h" "src/AssetManager.cpp" "include/SceneSerializer.h" "src/SceneSerializer.cpp")

target_sources(PhoenixEngine PRIVATE

    "../ThirdParty/ImGui/imgui.cpp"
    "../ThirdParty/ImGui/imgui_draw.cpp"
    "../ThirdParty/ImGui/imgui_tables.cpp"
    "../ThirdParty/ImGui/imgui_widgets.cpp"

    "../ThirdParty/ImGui/imgui_impl_glfw.cpp"
    "../ThirdParty/ImGui/imgui_impl_opengl3.cpp"

    "../ThirdParty/ImGui/imgui_demo.cpp"
)

target_include_directories(PhoenixEngine PUBLIC	include "../ThirdParty/ImGui")

find_package(glfw3 CONFIG REQUIRED)	
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(PhoenixEngine PUBLIC glfw glad::glad glm::glm)
