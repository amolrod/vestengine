#pragma once

#include <string>
#include <vector>
#include <utility>

#include <Scene/SceneObject.h>

namespace Vest {

class SceneHierarchyPanel {
public:
    explicit SceneHierarchyPanel(std::string title = "Scene Hierarchy");

    void SetSceneContext(std::vector<SceneObject>* objects, int* selectedIndex);
    void OnImGuiRender();

private:
    void DrawEntityNode(int index, SceneObject& object);

    std::string m_Title;
    std::vector<SceneObject>* m_SceneObjects = nullptr;
    int* m_SelectedIndex = nullptr;
};

}  // namespace Vest
