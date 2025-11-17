#include "GridRenderer.h"
#include "Rendering/Buffer.h"
#include "Rendering/RenderCommand.h"
#include <cmath>

namespace Vest {

GridRenderer::GridRenderer() 
    : m_GridSettings(), m_SnapSettings() {
}

void GridRenderer::Init() {
    // Create shader for grid rendering
    const std::string vertexSrc = R"(
        #version 410 core
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        
        uniform mat4 u_ViewProjection;
        
        out vec4 v_Color;
        
        void main() {
            v_Color = a_Color;
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
        }
    )";

    const std::string fragmentSrc = R"(
        #version 410 core
        layout(location = 0) out vec4 color;
        
        in vec4 v_Color;
        
        void main() {
            color = v_Color;
        }
    )";

    m_GridShader = Shader::Create("GridShader", vertexSrc, fragmentSrc);
    
    // Create VAO (will be updated dynamically)
    m_GridVA = VertexArray::Create();
    
    BufferLayout layout = {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"}
    };
    
    // Start with a minimal dummy buffer to avoid zero-size allocation
    std::vector<float> dummyData = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // vertex 1
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f  // vertex 2
    };
    auto vb = VertexBuffer::Create(dummyData.data(), static_cast<uint32_t>(dummyData.size() * sizeof(float)));
    vb->SetLayout(layout);
    m_GridVA->AddVertexBuffer(vb);
}

float GridRenderer::CalculateAdaptiveSpacing(float cameraZoom, const glm::vec2& viewportSize) const {
    if (!m_GridSettings.adaptive) {
        return m_GridSettings.spacing;
    }
    
    // Sanity checks
    if (cameraZoom <= 0.0f || viewportSize.y <= 0.0f) {
        return m_GridSettings.spacing;
    }
    
    // Calculate pixels per world unit
    float pixelsPerUnit = (viewportSize.y * 0.5f) / cameraZoom;
    
    // If pixelsPerUnit is too small, just return default spacing
    if (pixelsPerUnit < 0.001f) {
        return m_GridSettings.spacing;
    }
    
    float spacing = m_GridSettings.spacing;
    float currentPixelSpacing = spacing * pixelsPerUnit;
    
    // If too dense, increase spacing (with iteration limit)
    int iterations = 0;
    const int maxIterations = 20;
    while (currentPixelSpacing < m_GridSettings.minSpacingPixels && spacing < 100.0f && iterations < maxIterations) {
        spacing *= 2.0f;
        currentPixelSpacing = spacing * pixelsPerUnit;
        iterations++;
    }
    
    // If too sparse, decrease spacing (with iteration limit)
    iterations = 0;
    while (currentPixelSpacing > m_GridSettings.maxSpacingPixels && spacing > 0.1f && iterations < maxIterations) {
        spacing *= 0.5f;
        currentPixelSpacing = spacing * pixelsPerUnit;
        iterations++;
    }
    
    return spacing;
}


void GridRenderer::UpdateGridGeometry(const glm::vec3& cameraPosition,
                                       float visibleWidth,
                                       float visibleHeight,
                                       float actualSpacing) {
    m_GridVertices.clear();
    m_GridColors.clear();
    
    // Calculate grid bounds
    float halfWidth = visibleWidth * 0.5f;
    float halfHeight = visibleHeight * 0.5f;
    float left = cameraPosition.x - halfWidth;
    float right = cameraPosition.x + halfWidth;
    float bottom = cameraPosition.y - halfHeight;
    float top = cameraPosition.y + halfHeight;
    
    // Snap to grid
    left = std::floor(left / actualSpacing) * actualSpacing;
    right = std::ceil(right / actualSpacing) * actualSpacing;
    bottom = std::floor(bottom / actualSpacing) * actualSpacing;
    top = std::ceil(top / actualSpacing) * actualSpacing;
    
    auto addLine = [&](float x1, float y1, float x2, float y2, const glm::vec4& color) {
        // First vertex
        m_GridVertices.push_back(x1);
        m_GridVertices.push_back(y1);
        m_GridVertices.push_back(0.0f);
        m_GridColors.push_back(color.r);
        m_GridColors.push_back(color.g);
        m_GridColors.push_back(color.b);
        m_GridColors.push_back(color.a);
        
        // Second vertex
        m_GridVertices.push_back(x2);
        m_GridVertices.push_back(y2);
        m_GridVertices.push_back(0.0f);
        m_GridColors.push_back(color.r);
        m_GridColors.push_back(color.g);
        m_GridColors.push_back(color.b);
        m_GridColors.push_back(color.a);
    };
    
    // Draw vertical lines
    for (float x = left; x <= right; x += actualSpacing) {
        bool isMajor = std::abs(std::fmod(x, actualSpacing * m_GridSettings.majorLineInterval)) < 0.001f;
        bool isAxis = std::abs(x) < 0.001f;
        
        glm::vec4 color = isAxis ? m_GridSettings.axisXColor :
                         isMajor ? m_GridSettings.majorLineColor :
                         m_GridSettings.minorLineColor;
        
        addLine(x, bottom, x, top, color);
    }
    
    // Draw horizontal lines
    for (float y = bottom; y <= top; y += actualSpacing) {
        bool isMajor = std::abs(std::fmod(y, actualSpacing * m_GridSettings.majorLineInterval)) < 0.001f;
        bool isAxis = std::abs(y) < 0.001f;
        
        glm::vec4 color = isAxis ? m_GridSettings.axisYColor :
                         isMajor ? m_GridSettings.majorLineColor :
                         m_GridSettings.minorLineColor;
        
        addLine(left, y, right, y, color);
    }
    
    m_LastCameraPosition = cameraPosition;
    m_LastSpacing = actualSpacing;
    m_GeometryDirty = false;
}

void GridRenderer::RenderGrid(const glm::mat4& viewProjectionMatrix,
                              const glm::vec3& cameraPosition,
                              float cameraZoom,
                              const glm::vec2& viewportSize) {
    if (!m_GridSettings.enabled || !m_GridShader || !m_GridVA) {
        return;
    }
    
    // Don't render if viewport is invalid
    if (viewportSize.x <= 0.0f || viewportSize.y <= 0.0f || cameraZoom <= 0.0f) {
        return;
    }
    
    // Calculate adaptive spacing
    float actualSpacing = CalculateAdaptiveSpacing(cameraZoom, viewportSize);
    
    // Calculate visible area
    float visibleHeight = cameraZoom * 2.0f;
    float visibleWidth = visibleHeight * (viewportSize.x / viewportSize.y);
    
    // Check if we need to rebuild geometry
    glm::vec3 posDiff = cameraPosition - m_LastCameraPosition;
    float movementThreshold = actualSpacing * 0.5f;
    bool cameraMoved = glm::length(posDiff) > movementThreshold;
    bool spacingChanged = std::abs(actualSpacing - m_LastSpacing) > 0.001f;
    
    if (m_GeometryDirty || cameraMoved || spacingChanged) {
        UpdateGridGeometry(cameraPosition, visibleWidth, visibleHeight, actualSpacing);
        
        // Update vertex buffer
        if (!m_GridVertices.empty()) {
            std::vector<float> interleavedData;
            interleavedData.reserve(m_GridVertices.size() / 3 * 7); // 3 pos + 4 color
            
            for (size_t i = 0; i < m_GridVertices.size() / 3; ++i) {
                interleavedData.push_back(m_GridVertices[i * 3 + 0]);
                interleavedData.push_back(m_GridVertices[i * 3 + 1]);
                interleavedData.push_back(m_GridVertices[i * 3 + 2]);
                interleavedData.push_back(m_GridColors[i * 4 + 0]);
                interleavedData.push_back(m_GridColors[i * 4 + 1]);
                interleavedData.push_back(m_GridColors[i * 4 + 2]);
                interleavedData.push_back(m_GridColors[i * 4 + 3]);
            }
            
            auto& vertexBuffers = m_GridVA->GetVertexBuffers();
            if (!vertexBuffers.empty()) {
                vertexBuffers[0]->SetData(interleavedData.data(), 
                                         static_cast<uint32_t>(interleavedData.size() * sizeof(float)));
            }
        }
    }
    
    if (m_GridVertices.empty()) {
        return;
    }
    
    // Render grid
    m_GridShader->Bind();
    m_GridShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
    
    m_GridVA->Bind();
    RenderCommand::DrawLines(m_GridVA, static_cast<uint32_t>(m_GridVertices.size() / 3));
}

glm::vec3 GridRenderer::SnapToGrid(const glm::vec3& position) const {
    if (!ShouldSnap()) {
        return position;
    }
    
    float snap = m_SnapSettings.gridSnap;
    return glm::vec3(
        std::round(position.x / snap) * snap,
        std::round(position.y / snap) * snap,
        std::round(position.z / snap) * snap
    );
}

glm::vec2 GridRenderer::SnapToGrid(const glm::vec2& position) const {
    if (!ShouldSnap()) {
        return position;
    }
    
    float snap = m_SnapSettings.gridSnap;
    return glm::vec2(
        std::round(position.x / snap) * snap,
        std::round(position.y / snap) * snap
    );
}

} // namespace Vest
