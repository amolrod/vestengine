#include "Panels/SceneHierarchyPanel.h"

#include <imgui.h>

namespace Vest {

SceneHierarchyPanel::SceneHierarchyPanel(std::string title) : m_Title(std::move(title)) {}

void SceneHierarchyPanel::SetSceneContext(std::vector<SceneObject>* objects, int* selectedIndex) {
    m_SceneObjects = objects;
    m_SelectedIndex = selectedIndex;
}

void SceneHierarchyPanel::OnImGuiRender() {
    ImGui::Begin(m_Title.c_str());

    if (!m_SceneObjects || m_SceneObjects->empty()) {
        ImGui::TextUnformatted("No entities in scene");
    } else {
        for (int i = 0; i < static_cast<int>(m_SceneObjects->size()); ++i) {
            DrawEntityNode(i, (*m_SceneObjects)[static_cast<size_t>(i)]);
        }
    }

    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(int index, SceneObject& object) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (m_SelectedIndex && *m_SelectedIndex == index) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(index)), flags, "%s", object.name.c_str());
    if (ImGui::IsItemClicked()) {
        if (m_SelectedIndex) {
            *m_SelectedIndex = index;
        }
    }

    if (opened) {
        ImGui::TreePop();
    }
}

}  // namespace Vest
