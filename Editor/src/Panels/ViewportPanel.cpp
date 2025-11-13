#include "Panels/ViewportPanel.h"

#include <cstdint>

#include <imgui.h>

namespace Vest {

void ViewportPanel::OnImGuiRender() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    m_Focused = ImGui::IsWindowFocused();
    m_Hovered = ImGui::IsWindowHovered();

    ImVec2 viewportMinRegion = ImGui::GetCursorScreenPos();
    ImVec2 size = ImGui::GetContentRegionAvail();
    if (size.x > 0.0f && size.y > 0.0f) {
        m_ViewportSize = {size.x, size.y};
    }

    if (m_Framebuffer) {
        ImTextureID textureID = static_cast<ImTextureID>(m_Framebuffer->GetColorAttachmentRendererID());
        ImGui::Image(textureID, size, ImVec2(0, 1), ImVec2(1, 0));
    } else {
        ImGui::TextUnformatted("Framebuffer not available");
    }

    ImVec2 viewportMaxRegion = {viewportMinRegion.x + size.x, viewportMinRegion.y + size.y};
    m_Bounds[0] = {viewportMinRegion.x, viewportMinRegion.y};
    m_Bounds[1] = {viewportMaxRegion.x, viewportMaxRegion.y};

    ImGui::End();
    ImGui::PopStyleVar();
}

}  // namespace Vest
