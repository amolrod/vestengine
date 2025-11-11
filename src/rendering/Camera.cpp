/**
 * @file Camera.cpp
 * @brief Implementación del sistema de cámaras
 */

#include "Camera.h"
#include "../core/Logger.h"

#include <algorithm>

namespace Engine {

// =============================================================================
// Perspective Camera
// =============================================================================

PerspectiveCamera::PerspectiveCamera(const PerspectiveCameraConfig& config)
    : m_position(config.position)
    , m_worldUp(config.worldUp)
    , m_yaw(config.yaw)
    , m_pitch(config.pitch)
    , m_movementSpeed(config.movementSpeed)
    , m_mouseSensitivity(config.mouseSensitivity)
    , m_zoom(config.zoom)
    , m_fov(config.fov)
    , m_aspectRatio(config.aspectRatio)
    , m_nearClip(config.nearClip)
    , m_farClip(config.farClip) {
    
    UpdateCameraVectors();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
    
    LOG_DEBUG("PerspectiveCamera creada (Pos: [{:.2f}, {:.2f}, {:.2f}], FOV: {:.1f}°)", 
              m_position.x, m_position.y, m_position.z, m_fov);
}

void PerspectiveCamera::SetPosition(const glm::vec3& position) {
    m_position = position;
    UpdateViewMatrix();
}

void PerspectiveCamera::SetYaw(float yaw) {
    m_yaw = yaw;
    UpdateCameraVectors();
    UpdateViewMatrix();
}

void PerspectiveCamera::SetPitch(float pitch) {
    m_pitch = pitch;
    
    // Constrain pitch
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;
    
    UpdateCameraVectors();
    UpdateViewMatrix();
}

void PerspectiveCamera::SetFOV(float fov) {
    m_fov = std::clamp(fov, 1.0f, 120.0f);
    UpdateProjectionMatrix();
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio) {
    m_aspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void PerspectiveCamera::LookAt(const glm::vec3& target) {
    glm::vec3 direction = glm::normalize(target - m_position);
    
    // Calcular yaw y pitch desde el vector de dirección
    m_yaw = glm::degrees(atan2(direction.z, direction.x));
    m_pitch = glm::degrees(asin(direction.y));
    
    UpdateCameraVectors();
    UpdateViewMatrix();
}

// =============================================================================
// Camera Controls
// =============================================================================

void PerspectiveCamera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    
    switch (direction) {
        case CameraMovement::Forward:
            m_position += m_front * velocity;
            break;
        case CameraMovement::Backward:
            m_position -= m_front * velocity;
            break;
        case CameraMovement::Left:
            m_position -= m_right * velocity;
            break;
        case CameraMovement::Right:
            m_position += m_right * velocity;
            break;
        case CameraMovement::Up:
            m_position += m_up * velocity;
            break;
        case CameraMovement::Down:
            m_position -= m_up * velocity;
            break;
    }
    
    UpdateViewMatrix();
}

void PerspectiveCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;
    
    m_yaw += xoffset;
    m_pitch += yoffset;
    
    // Constrain pitch para evitar gimbal lock
    if (constrainPitch) {
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
    }
    
    UpdateCameraVectors();
    UpdateViewMatrix();
}

void PerspectiveCamera::ProcessMouseScroll(float yoffset) {
    m_zoom -= yoffset;
    m_zoom = std::clamp(m_zoom, 1.0f, 120.0f);
    
    // Zoom afecta al FOV
    m_fov = m_zoom;
    UpdateProjectionMatrix();
}

// =============================================================================
// Private Methods
// =============================================================================

void PerspectiveCamera::UpdateCameraVectors() {
    // Calcular el vector front desde yaw/pitch
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    
    // Recalcular right y up
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void PerspectiveCamera::UpdateViewMatrix() {
    m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

void PerspectiveCamera::UpdateProjectionMatrix() {
    m_projectionMatrix = glm::perspective(
        glm::radians(m_fov), 
        m_aspectRatio, 
        m_nearClip, 
        m_farClip
    );
}

// =============================================================================
// Orthographic Camera
// =============================================================================

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_viewMatrix(1.0f)
    , m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)) {
    
    LOG_DEBUG("OrthographicCamera creada (Bounds: [{}, {}] x [{}, {}])", 
              left, right, bottom, top);
}

void OrthographicCamera::SetPosition(const glm::vec3& position) {
    m_position = position;
    UpdateViewMatrix();
}

void OrthographicCamera::SetRotation(float rotation) {
    m_rotation = rotation;
    UpdateViewMatrix();
}

void OrthographicCamera::UpdateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);
    transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0, 0, 1));
    
    m_viewMatrix = glm::inverse(transform);
}

} // namespace Engine
