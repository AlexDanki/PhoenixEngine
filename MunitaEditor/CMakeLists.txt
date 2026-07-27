add_executable(MunitaEditor)

target_sources(MunitaEditor PRIVATE src/main.cpp 
    "include/Core/EditorApplication.h" 
    "src/Core/EditorApplication.cpp" "include/Panels/HierarchyPanel.h" "src/Panels/HierachyPanel.cpp" "include/Panels/EditorPanel.h" "include/Panels/InspectorPanel.h" "src/Panels/InspectorPanel.cpp" "include/Panels/AssetBrowserPanel.h" "src/Panels/AssetBrowserPanel.cpp" "include/Panels/SceneViewPanel.h" "src/Panels/SceneViewPanel.cpp" "include/Panels/GameViewPanel.h" "src/Panels/GameViewPanel.cpp" "include/Graphics/EditorCamera.h" "src/Graphics/EditorCamera.cpp")

target_include_directories(MunitaEditor PRIVATE include
${CMAKE_SOURCE_DIR}/MunitaEditor/include/Core)

target_link_libraries(MunitaEditor PRIVATE MunitaEngine)