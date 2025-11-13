#include "Panels/PropertiesPanel.h"

#include <imgui.h>

namespace Vest {

void PropertiesPanel::OnImGuiRender() {
    ImGui::Begin(m_Title.c_str());

    if (!m_SceneObjects || !m_SelectedIndex || *m_SelectedIndex < 0 ||
        *m_SelectedIndex >= static_cast<int>(m_SceneObjects->size())) {
        ImGui::TextUnformatted("Select an entity from the hierarchy");
        ImGui::End();
        return;
    }

    SceneObject& object = (*m_SceneObjects)[static_cast<size_t>(*m_SelectedIndex)];

    ImGui::Text("Entity: %s", object.name.c_str());
    ImGui::Separator();

    ImGui::DragFloat3("Position", &object.position.x, 0.01f);
    ImGui::DragFloat3("Scale", &object.scale.x, 0.01f, 0.1f, 10.0f);
    ImGui::ColorEdit4("Color", &object.color.x);
    ImGui::Checkbox("Textured", &object.textured);

    ImGui::End();
}

}  // namespace Vest
