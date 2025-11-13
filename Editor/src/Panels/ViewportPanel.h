#pragma once

#include <glm/glm.hpp>

#include "Core/Base.h"
#include "Rendering/Framebuffer.h"

namespace Vest {

class ViewportPanel {
public:
    void SetFramebuffer(const Ref<Framebuffer>& framebuffer) { m_Framebuffer = framebuffer; }

    void OnImGuiRender();

    const glm::vec2& GetViewportSize() const { return m_ViewportSize; }
    bool IsFocused() const { return m_Focused; }
    bool IsHovered() const { return m_Hovered; }
    const glm::vec2* GetBounds() const { return m_Bounds; }

private:
    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize = {0.0f, 0.0f};
    bool m_Focused = false;
    bool m_Hovered = false;
    glm::vec2 m_Bounds[2] = {glm::vec2(0.0f), glm::vec2(0.0f)};
};

}  // namespace Vest
