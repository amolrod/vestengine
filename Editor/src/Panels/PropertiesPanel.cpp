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
    ImGui::DragFloat3("Rotation", &object.rotation.x, 0.1f, -180.0f, 180.0f);
    ImGui::DragFloat3("Scale", &object.scale.x, 0.01f, 0.1f, 10.0f);
    ImGui::ColorEdit4("Color", &object.color.x);

    const char* meshOptions[] = {"Triangle", "Quad"};
    int meshIndex = static_cast<int>(object.mesh);
    if (ImGui::Combo("Mesh", &meshIndex, meshOptions, IM_ARRAYSIZE(meshOptions))) {
        object.mesh = static_cast<SceneObject::MeshType>(meshIndex);
        if (object.mesh != SceneObject::MeshType::Quad) {
            object.textured = false;
        }
    }

    bool texturedEnabled = object.mesh == SceneObject::MeshType::Quad;
    ImGui::BeginDisabled(!texturedEnabled);
    ImGui::Checkbox("Textured", &object.textured);
    ImGui::EndDisabled();

    ImGui::End();
}

}  // namespace Vest
