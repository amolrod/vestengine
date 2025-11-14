#include "EditorCamera.h"

#include <imgui.h>
#include <algorithm>

#include "Core/Input.h"
#include "Core/Log.h"

namespace Vest {

EditorCamera::EditorCamera(float aspectRatio, float zoom)
    : m_AspectRatio(aspectRatio)
    , m_Zoom(zoom)
    , m_TargetZoom(zoom)
{
    RecalculateMatrices();
}

void EditorCamera::OnUpdate(Timestep ts) {
    // Smoothing for position and zoom
    if (m_SmoothingEnabled) {
        UpdatePanSmoothing(ts.GetSeconds());
        UpdateZoomSmoothing(ts.GetSeconds());
    }
    
    RecalculateMatrices();
}

void EditorCamera::OnMouseScroll(float delta) {
    m_TargetZoom -= delta * m_ZoomSpeed;
    m_TargetZoom = glm::clamp(m_TargetZoom, m_MinZoom, m_MaxZoom);
    
    if (!m_SmoothingEnabled) {
        m_Zoom = m_TargetZoom;
    }
}

void EditorCamera::Pan(const glm::vec2& delta) {
    const float speed = m_PanSpeed * m_Zoom * 0.001f;
    m_TargetPosition.x -= delta.x * speed;
    m_TargetPosition.y += delta.y * speed;
    
    if (!m_SmoothingEnabled) {
        m_Position = m_TargetPosition;
    }
}

void EditorCamera::FocusOn(const glm::vec3& position) {
    m_TargetPosition = position;
    
    if (!m_SmoothingEnabled) {
        m_Position = m_TargetPosition;
    }
    
    VEST_CORE_TRACE("Camera focused on position: ({0}, {1}, {2})", 
        position.x, position.y, position.z);
}

void EditorCamera::FrameBounds(const glm::vec3& min, const glm::vec3& max) {
    // Calculate center and size
    glm::vec3 center = (min + max) * 0.5f;
    glm::vec3 size = max - min;
    
    // Calculate zoom to fit bounds
    float requiredZoomX = size.x / (2.0f * m_AspectRatio);
    float requiredZoomY = size.y / 2.0f;
    float requiredZoom = std::max(requiredZoomX, requiredZoomY) * 1.2f; // 20% padding
    
    m_TargetPosition = center;
    m_TargetZoom = glm::clamp(requiredZoom, m_MinZoom, m_MaxZoom);
    
    if (!m_SmoothingEnabled) {
        m_Position = m_TargetPosition;
        m_Zoom = m_TargetZoom;
    }
    
    VEST_CORE_TRACE("Camera framed bounds - Center: ({0}, {1}), Zoom: {2}", 
        center.x, center.y, m_TargetZoom);
}

void EditorCamera::SetAspectRatio(float aspectRatio) {
    m_AspectRatio = aspectRatio;
    RecalculateMatrices();
}

glm::vec2 EditorCamera::ScreenToWorld(const glm::vec2& screenPos, const glm::vec2& viewportSize) const {
    // Normalize screen coordinates to [-1, 1]
    glm::vec2 normalized;
    normalized.x = (screenPos.x / viewportSize.x) * 2.0f - 1.0f;
    normalized.y = 1.0f - (screenPos.y / viewportSize.y) * 2.0f;
    
    // Apply inverse view-projection
    glm::mat4 invViewProj = glm::inverse(m_ViewProjectionMatrix);
    glm::vec4 worldPos = invViewProj * glm::vec4(normalized, 0.0f, 1.0f);
    
    return glm::vec2(worldPos.x, worldPos.y);
}

glm::vec2 EditorCamera::WorldToScreen(const glm::vec2& worldPos, const glm::vec2& viewportSize) const {
    glm::vec4 clipSpace = m_ViewProjectionMatrix * glm::vec4(worldPos, 0.0f, 1.0f);
    
    // Convert from clip space [-1, 1] to screen space [0, viewportSize]
    glm::vec2 normalized;
    normalized.x = (clipSpace.x + 1.0f) * 0.5f;
    normalized.y = (1.0f - clipSpace.y) * 0.5f;
    
    return normalized * viewportSize;
}

void EditorCamera::RecalculateMatrices() {
    const float orthoHeight = m_Zoom;
    const float orthoWidth = orthoHeight * m_AspectRatio;
    
    m_ProjectionMatrix = glm::ortho(
        -orthoWidth, orthoWidth, 
        -orthoHeight, orthoHeight, 
        -1.0f, 10.0f
    );
    
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), -m_Position);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void EditorCamera::UpdatePanSmoothing(float ts) {
    // Smooth interpolation towards target position
    glm::vec3 delta = m_TargetPosition - m_Position;
    float distance = glm::length(delta);
    
    if (distance > 0.0001f) {
        float t = std::min(1.0f, m_PositionSmoothness * ts);
        m_Position += delta * t;
    } else {
        m_Position = m_TargetPosition;
    }
}

void EditorCamera::UpdateZoomSmoothing(float ts) {
    // Smooth interpolation towards target zoom
    float delta = m_TargetZoom - m_Zoom;
    
    if (std::abs(delta) > 0.0001f) {
        float t = std::min(1.0f, m_ZoomSmoothness * ts);
        m_Zoom += delta * t;
    } else {
        m_Zoom = m_TargetZoom;
    }
}

}  // namespace Vest
