/**
 * @file Camera.h
 * @brief Sistema de cámaras 3D (Perspective y Orthographic)
 * 
 * Jerarquía:
 * - Camera: Clase base abstracta
 * - PerspectiveCamera: Cámara 3D con perspectiva
 * - OrthographicCamera: Cámara 2D/isométrica
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

/**
 * @enum CameraMovement
 * @brief Direcciones de movimiento de la cámara
 */
enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

/**
 * @class Camera
 * @brief Clase base abstracta para cámaras
 * 
 * Responsabilidad: Calcular matrices de vista y proyección
 */
class Camera {
public:
    virtual ~Camera() = default;
    
    /**
     * @brief Obtiene la matriz de vista (view matrix)
     */
    virtual const glm::mat4& GetViewMatrix() const = 0;
    
    /**
     * @brief Obtiene la matriz de proyección (projection matrix)
     */
    virtual const glm::mat4& GetProjectionMatrix() const = 0;
    
    /**
     * @brief Obtiene la matriz combinada ViewProjection
     */
    virtual glm::mat4 GetViewProjectionMatrix() const {
        return GetProjectionMatrix() * GetViewMatrix();
    }
    
    /**
     * @brief Obtiene la posición de la cámara
     */
    virtual const glm::vec3& GetPosition() const = 0;
    
    /**
     * @brief Establece la posición de la cámara
     */
    virtual void SetPosition(const glm::vec3& position) = 0;
};

// =============================================================================
// Perspective Camera
// =============================================================================

/**
 * @struct PerspectiveCameraConfig
 * @brief Configuración de cámara perspectiva
 */
struct PerspectiveCameraConfig {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -90.0f;          // Ángulo horizontal (grados)
    float pitch = 0.0f;          // Ángulo vertical (grados)
    float fov = 45.0f;           // Field of View (grados)
    float aspectRatio = 16.0f / 9.0f;
    float nearClip = 0.1f;
    float farClip = 100.0f;
    float movementSpeed = 2.5f;
    float mouseSensitivity = 0.1f;
    float zoom = 45.0f;
};

/**
 * @class PerspectiveCamera
 * @brief Cámara 3D con perspectiva y controles FPS
 * 
 * Implementa cámara estilo FPS con:
 * - Movimiento WASD + Q/E (arriba/abajo)
 * - Rotación con mouse
 * - Zoom con scroll
 * 
 * Uso:
 *   auto camera = PerspectiveCamera(config);
 *   camera.ProcessKeyboard(CameraMovement::Forward, deltaTime);
 *   camera.ProcessMouseMovement(xoffset, yoffset);
 *   shader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
 */
class PerspectiveCamera : public Camera {
public:
    /**
     * @brief Constructor con configuración
     */
    PerspectiveCamera(const PerspectiveCameraConfig& config = PerspectiveCameraConfig());
    
    /**
     * @brief Destructor
     */
    ~PerspectiveCamera() override = default;
    
    // =========================================================================
    // Camera Interface Implementation
    // =========================================================================
    
    const glm::mat4& GetViewMatrix() const override { return m_viewMatrix; }
    const glm::mat4& GetProjectionMatrix() const override { return m_projectionMatrix; }
    const glm::vec3& GetPosition() const override { return m_position; }
    void SetPosition(const glm::vec3& position) override;
    
    // =========================================================================
    // Camera Controls
    // =========================================================================
    
    /**
     * @brief Procesa input de teclado
     * @param direction Dirección del movimiento
     * @param deltaTime Tiempo transcurrido desde el último frame
     */
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    
    /**
     * @brief Procesa movimiento del mouse
     * @param xoffset Desplazamiento horizontal
     * @param yoffset Desplazamiento vertical
     * @param constrainPitch Si true, limita pitch a [-89, 89] grados
     */
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    
    /**
     * @brief Procesa scroll del mouse (zoom)
     * @param yoffset Cantidad de scroll
     */
    void ProcessMouseScroll(float yoffset);
    
    // =========================================================================
    // Getters/Setters
    // =========================================================================
    
    const glm::vec3& GetFront() const { return m_front; }
    const glm::vec3& GetRight() const { return m_right; }
    const glm::vec3& GetUp() const { return m_up; }
    
    float GetYaw() const { return m_yaw; }
    float GetPitch() const { return m_pitch; }
    float GetFOV() const { return m_fov; }
    float GetZoom() const { return m_zoom; }
    
    void SetYaw(float yaw);
    void SetPitch(float pitch);
    void SetFOV(float fov);
    
    /**
     * @brief Actualiza el aspect ratio (útil para resize de ventana)
     */
    void SetAspectRatio(float aspectRatio);
    
    /**
     * @brief Hace que la cámara mire hacia un punto
     */
    void LookAt(const glm::vec3& target);

private:
    /**
     * @brief Recalcula los vectores de la cámara desde yaw/pitch
     */
    void UpdateCameraVectors();
    
    /**
     * @brief Recalcula la matriz de vista
     */
    void UpdateViewMatrix();
    
    /**
     * @brief Recalcula la matriz de proyección
     */
    void UpdateProjectionMatrix();

private:
    // Matrices
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    
    // Vectores de la cámara
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    
    // Ángulos de Euler (grados)
    float m_yaw;
    float m_pitch;
    
    // Opciones de cámara
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;
    float m_fov;
    float m_aspectRatio;
    float m_nearClip;
    float m_farClip;
};

// =============================================================================
// Orthographic Camera (para futura implementación)
// =============================================================================

/**
 * @class OrthographicCamera
 * @brief Cámara ortográfica (2D/isométrica)
 * 
 * Útil para:
 * - UI
 * - Juegos 2D
 * - Vistas isométricas
 */
class OrthographicCamera : public Camera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);
    ~OrthographicCamera() override = default;
    
    const glm::mat4& GetViewMatrix() const override { return m_viewMatrix; }
    const glm::mat4& GetProjectionMatrix() const override { return m_projectionMatrix; }
    const glm::vec3& GetPosition() const override { return m_position; }
    void SetPosition(const glm::vec3& position) override;
    
    void SetRotation(float rotation);
    float GetRotation() const { return m_rotation; }

private:
    void UpdateViewMatrix();

private:
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    glm::vec3 m_position = glm::vec3(0.0f);
    float m_rotation = 0.0f;
};

} // namespace Engine
