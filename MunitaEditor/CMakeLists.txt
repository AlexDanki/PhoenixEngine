add_executable(MunitaEditor)

target_sources(MunitaEditor PRIVATE src/main.cpp 
    "include/Core/EditorAppication.h" 
    "src/Core/EditorApplication.cpp" "include/Panels/HierarchyPanel.h" "src/Panels/HierachyPanel.cpp" "include/Panels/EditorPanel.h" "include/Panels/InspectorPanel.h" "src/Panels/InspectorPanel.cpp" "include/Panels/AssetBrowserPanel.h" "src/Panels/AssetBrowser.cpp" "include/Panels/SceneViewPanel.h" "src/Panels/SceneViewPanel.cpp" "include/Panels/GameViewPanel.h" "src/Panels/GameViewPanel.cpp")

target_include_directories(MunitaEditor PRIVATE include)

target_link_libraries(MunitaEditor PRIVATE MunitaEngine)