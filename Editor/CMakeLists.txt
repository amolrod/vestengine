add_executable(VestEditor
    src/EditorApplication.cpp
    src/EditorLayer.cpp
    src/EditorCamera.cpp
    src/Rendering/SelectionRenderer.cpp
    src/Panels/ViewportPanel.cpp
    src/Panels/SceneHierarchyPanel.cpp
    src/Panels/PropertiesPanel.cpp
    src/Panels/ContentBrowserPanel.cpp
    src/Panels/StatsPanel.cpp
    ${CMAKE_SOURCE_DIR}/external/imguizmo/ImGuizmo.cpp
)

target_include_directories(VestEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external/imguizmo
)

target_link_libraries(VestEditor PRIVATE VestEngine)
