#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Timestep.h"

namespace Vest {

/**
 * @brief Editor-specific orthographic camera with pan, zoom, and smooth controls
 * 
 * Features:
 * - Smooth pan with middle/right mouse button
 * - Zoom with mouse wheel (clamped to prevent extreme values)
 * - Keyboard WASD movement
 * - Focus on position (center camera on point)
 * - Aspect ratio handling
 * - Frame bounds (fit camera to see entire scene)
 */
class EditorCamera {
public:
    EditorCamera() = default;
    EditorCamera(float aspectRatio, float zoom = 1.5f);

    void OnUpdate(Timestep ts);
    void OnMouseScroll(float delta);
    
    // Camera movement
    void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateMatrices(); }
    void SetZoom(float zoom) { m_Zoom = glm::clamp(zoom, m_MinZoom, m_MaxZoom); RecalculateMatrices(); }
    void Pan(const glm::vec2& delta);
    void FocusOn(const glm::vec3& position);
    void FrameBounds(const glm::vec3& min, const glm::vec3& max);
    
    // Getters
    const glm::vec3& GetPosition() const { return m_Position; }
    float GetZoom() const { return m_Zoom; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    
    // Configuration
    void SetAspectRatio(float aspectRatio);
    void SetZoomLimits(float min, float max) { m_MinZoom = min; m_MaxZoom = max; }
    void SetPanSpeed(float speed) { m_PanSpeed = speed; }
    void SetZoomSpeed(float speed) { m_ZoomSpeed = speed; }
    void SetKeyboardSpeed(float speed) { m_KeyboardSpeed = speed; }
    void SetSmoothingEnabled(bool enabled) { m_SmoothingEnabled = enabled; }
    
    // Screen to world conversion
    glm::vec2 ScreenToWorld(const glm::vec2& screenPos, const glm::vec2& viewportSize) const;
    glm::vec2 WorldToScreen(const glm::vec2& worldPos, const glm::vec2& viewportSize) const;

private:
    void RecalculateMatrices();
    void UpdatePanSmoothing(float ts);
    void UpdateZoomSmoothing(float ts);

private:
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_Zoom = 1.5f;
    float m_AspectRatio = 16.0f / 9.0f;
    
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
    
    // Camera limits
    float m_MinZoom = 0.25f;
    float m_MaxZoom = 10.0f;
    
    // Movement speeds
    float m_PanSpeed = 1.0f;
    float m_ZoomSpeed = 0.1f;
    float m_KeyboardSpeed = 2.0f;
    
    // Smoothing
    bool m_SmoothingEnabled = true;
    glm::vec3 m_TargetPosition = {0.0f, 0.0f, 0.0f};
    float m_TargetZoom = 1.5f;
    float m_PositionSmoothness = 10.0f;
    float m_ZoomSmoothness = 8.0f;
};

}  // namespace Vest
