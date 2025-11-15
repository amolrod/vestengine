#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"

namespace Vest {

struct GridSettings {
    bool enabled = true;
    float spacing = 1.0f;          // Grid cell size
    float majorLineInterval = 5.0f; // Major lines every N cells
    
    glm::vec4 minorLineColor = {0.3f, 0.3f, 0.3f, 0.4f};
    glm::vec4 majorLineColor = {0.4f, 0.4f, 0.4f, 0.6f};
    glm::vec4 axisXColor = {0.8f, 0.2f, 0.2f, 0.8f};
    glm::vec4 axisYColor = {0.2f, 0.8f, 0.2f, 0.8f};
    
    float minorLineWidth = 1.0f;
    float majorLineWidth = 1.5f;
    float axisLineWidth = 2.0f;
    
    // Adaptive grid - change spacing based on zoom
    bool adaptive = true;
    float minSpacingPixels = 20.0f; // Minimum pixels between grid lines
    float maxSpacingPixels = 100.0f; // Maximum pixels between grid lines
};

struct SnapSettings {
    bool enabled = false;
    float gridSnap = 0.5f;         // Snap to this grid size
    bool snapToGrid = true;
    bool snapToObjects = false;    // Future: snap to other objects
    float snapThreshold = 0.3f;    // Distance threshold for snapping
};

class GridRenderer {
public:
    GridRenderer();
    ~GridRenderer() = default;

    void Init();
    
    // Render grid in world space
    void RenderGrid(const glm::mat4& viewProjectionMatrix, 
                    const glm::vec3& cameraPosition,
                    float cameraZoom,
                    const glm::vec2& viewportSize);
    
    // Snap a position to grid
    glm::vec3 SnapToGrid(const glm::vec3& position) const;
    glm::vec2 SnapToGrid(const glm::vec2& position) const;
    
    // Check if a value should snap
    bool ShouldSnap() const { return m_SnapSettings.enabled && m_SnapSettings.snapToGrid; }
    
    GridSettings& GetGridSettings() { return m_GridSettings; }
    const GridSettings& GetGridSettings() const { return m_GridSettings; }
    
    SnapSettings& GetSnapSettings() { return m_SnapSettings; }
    const SnapSettings& GetSnapSettings() const { return m_SnapSettings; }

private:
    void UpdateGridGeometry(const glm::vec3& cameraPosition, 
                           float visibleWidth, 
                           float visibleHeight,
                           float actualSpacing);
    
    float CalculateAdaptiveSpacing(float cameraZoom, const glm::vec2& viewportSize) const;

    GridSettings m_GridSettings;
    SnapSettings m_SnapSettings;
    
    Ref<Shader> m_GridShader;
    Ref<VertexArray> m_GridVA;
    
    std::vector<float> m_GridVertices;
    std::vector<float> m_GridColors;
    
    bool m_GeometryDirty = true;
    glm::vec3 m_LastCameraPosition = glm::vec3(0.0f);
    float m_LastSpacing = 1.0f;
};

} // namespace Vest
