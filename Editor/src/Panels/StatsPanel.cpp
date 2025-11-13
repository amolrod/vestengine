#include "Panels/StatsPanel.h"

#include <imgui.h>

namespace Vest {

void StatsPanel::OnImGuiRender() {
    ImGui::Begin(m_Title.c_str());
    ImGui::Text("FPS: %.2f", m_FPS);
    ImGui::Text("Draw Calls: %u", m_DrawCalls);
    ImGui::End();
}

}  // namespace Vest
