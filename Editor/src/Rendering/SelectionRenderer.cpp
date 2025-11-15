#include "SelectionRenderer.h"
#include <cmath>

namespace Vest {

SelectionRenderer::SelectionRenderer() 
    : m_Style(), m_AnimationTime(0.0f) {
}

void SelectionRenderer::Update(float deltaTime) {
    if (m_Style.animateSelection) {
        m_AnimationTime += deltaTime * m_Style.pulseSpeed;
    }
}

ImU32 SelectionRenderer::GetColor(SelectionState state, float alpha) const {
    glm::vec4 color;
    
    switch (state) {
        case SelectionState::None:
            return IM_COL32(0, 0, 0, 0);
            
        case SelectionState::Hovered:
            color = m_Style.hoveredColor;
            break;
            
        case SelectionState::Selected: {
            color = m_Style.selectedColor;
            // Animate pulse
            if (m_Style.animateSelection) {
                float pulse = std::sin(m_AnimationTime) * m_Style.pulseAmount;
                color.w = glm::clamp(color.w + pulse, 0.5f, 1.0f);
            }
            break;
        }
            
        case SelectionState::HoveredAndSelected:
            color = m_Style.selectedHoveredColor;
            break;
    }
    
    color.w *= alpha;
    return IM_COL32(
        static_cast<int>(color.r * 255.0f),
        static_cast<int>(color.g * 255.0f),
        static_cast<int>(color.b * 255.0f),
        static_cast<int>(color.w * 255.0f)
    );
}

float SelectionRenderer::GetThickness(SelectionState state) const {
    switch (state) {
        case SelectionState::Hovered:
            return m_Style.hoveredThickness;
        case SelectionState::Selected:
            return m_Style.selectedThickness;
        case SelectionState::HoveredAndSelected:
            return m_Style.selectedHoveredThickness;
        default:
            return 1.0f;
    }
}

float SelectionRenderer::GetScale(SelectionState state) const {
    switch (state) {
        case SelectionState::Hovered:
            return m_Style.hoveredScale;
        case SelectionState::Selected:
        case SelectionState::HoveredAndSelected:
            return m_Style.outlineScale;
        default:
            return 1.0f;
    }
}

void SelectionRenderer::DrawOutline(ImDrawList* drawList, 
                                     const glm::vec2* screenCorners,
                                     const glm::vec2& viewportOffset,
                                     SelectionState state) {
    if (state == SelectionState::None || !drawList) {
        return;
    }

    ImU32 color = GetColor(state);
    float thickness = GetThickness(state);
    
    // Dibujar las 4 líneas del outline
    for (int i = 0; i < 4; ++i) {
        glm::vec2 p1 = screenCorners[i];
        glm::vec2 p2 = screenCorners[(i + 1) % 4];
        
        ImVec2 imP1(viewportOffset.x + p1.x, viewportOffset.y + p1.y);
        ImVec2 imP2(viewportOffset.x + p2.x, viewportOffset.y + p2.y);
        
        drawList->AddLine(imP1, imP2, color, thickness);
    }
    
    // Añadir corners más gruesos para mejor visibilidad
    if (state == SelectionState::Selected || state == SelectionState::HoveredAndSelected) {
        float cornerSize = 8.0f;
        for (int i = 0; i < 4; ++i) {
            glm::vec2 corner = screenCorners[i];
            ImVec2 center(viewportOffset.x + corner.x, viewportOffset.y + corner.y);
            drawList->AddCircleFilled(center, cornerSize * 0.5f, color);
        }
    }
}

void SelectionRenderer::DrawHighlight(ImDrawList* drawList,
                                       const glm::vec2* screenCorners,
                                       const glm::vec2& viewportOffset,
                                       SelectionState state) {
    if (state == SelectionState::None || !drawList) {
        return;
    }

    // Solo hover tiene fill semi-transparente
    if (state == SelectionState::Hovered) {
        ImU32 fillColor = GetColor(state, 0.15f); // Muy transparente
        
        ImVec2 points[4];
        for (int i = 0; i < 4; ++i) {
            points[i] = ImVec2(
                viewportOffset.x + screenCorners[i].x,
                viewportOffset.y + screenCorners[i].y
            );
        }
        
        drawList->AddConvexPolyFilled(points, 4, fillColor);
    }
}

void SelectionRenderer::DrawSelectionBox(ImDrawList* drawList,
                                          const glm::vec2& startPos,
                                          const glm::vec2& endPos,
                                          const glm::vec2& viewportOffset) {
    if (!drawList) {
        return;
    }

    ImVec2 p1(viewportOffset.x + startPos.x, viewportOffset.y + startPos.y);
    ImVec2 p2(viewportOffset.x + endPos.x, viewportOffset.y + endPos.y);
    
    // Fondo semi-transparente
    ImU32 fillColor = IM_COL32(100, 150, 255, 30);
    drawList->AddRectFilled(p1, p2, fillColor);
    
    // Border
    ImU32 borderColor = IM_COL32(100, 150, 255, 200);
    drawList->AddRect(p1, p2, borderColor, 0.0f, 0, 2.0f);
}

} // namespace Vest
