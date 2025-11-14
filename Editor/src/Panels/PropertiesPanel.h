#pragma once

#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include <Scene/SceneObject.h>

namespace Vest {

class PropertiesPanel {
public:
    explicit PropertiesPanel(std::string title = "Properties") : m_Title(std::move(title)) {}

    void SetSceneContext(std::vector<SceneObject>* objects, int* selectedIndex) {
        m_SceneObjects = objects;
        m_SelectedIndex = selectedIndex;
    }

    void OnImGuiRender();

private:
    std::string m_Title;
    std::vector<SceneObject>* m_SceneObjects = nullptr;
    int* m_SelectedIndex = nullptr;
};

}  // namespace Vest
