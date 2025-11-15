#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include <vector>

namespace Vest {

enum class SelectionState {
    None,
    Hovered,
    Selected,
    HoveredAndSelected
};

struct SelectionStyle {
    glm::vec4 selectedColor = {1.0f, 0.8f, 0.0f, 1.0f};      // Amarillo/dorado
    glm::vec4 hoveredColor = {0.3f, 0.6f, 1.0f, 0.6f};       // Azul claro
    glm::vec4 selectedHoveredColor = {1.0f, 0.5f, 0.0f, 1.0f}; // Naranja
    
    float selectedThickness = 2.5f;
    float hoveredThickness = 2.0f;
    float selectedHoveredThickness = 3.0f;
    
    float outlineScale = 1.05f;
    float hoveredScale = 1.03f;
    
    bool animateSelection = true;
    float pulseSpeed = 2.0f;
    float pulseAmount = 0.15f;
};

class SelectionRenderer {
public:
    SelectionRenderer();
    ~SelectionRenderer() = default;

    void Update(float deltaTime);
    
    // Dibuja outline en el viewport usando ImGui
    void DrawOutline(ImDrawList* drawList, 
                     const glm::vec2* screenCorners, 
                     const glm::vec2& viewportOffset,
                     SelectionState state);
    
    // Dibuja un highlight sobre toda la entidad
    void DrawHighlight(ImDrawList* drawList,
                       const glm::vec2* screenCorners,
                       const glm::vec2& viewportOffset,
                       SelectionState state);
    
    // Dibuja un box de selección múltiple
    void DrawSelectionBox(ImDrawList* drawList,
                          const glm::vec2& startPos,
                          const glm::vec2& endPos,
                          const glm::vec2& viewportOffset);

    SelectionStyle& GetStyle() { return m_Style; }
    const SelectionStyle& GetStyle() const { return m_Style; }

private:
    SelectionStyle m_Style;
    float m_AnimationTime = 0.0f;
    
    ImU32 GetColor(SelectionState state, float alpha = 1.0f) const;
    float GetThickness(SelectionState state) const;
    float GetScale(SelectionState state) const;
};

} // namespace Vest
