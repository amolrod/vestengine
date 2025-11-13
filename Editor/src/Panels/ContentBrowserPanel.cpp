#include "Panels/ContentBrowserPanel.h"

#include <imgui.h>

namespace Vest {

ContentBrowserPanel::ContentBrowserPanel(std::string title) : m_Title(std::move(title)) {
    m_Entries = {"Scenes", "Textures", "Scripts", "Shaders"};
}

void ContentBrowserPanel::OnImGuiRender() {
    ImGui::Begin(m_Title.c_str());

    for (const auto& entry : m_Entries) {
        ImGui::Selectable(entry.c_str());
    }

    ImGui::End();
}

}  // namespace Vest
