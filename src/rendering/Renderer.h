#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <memory>

namespace Engine {

/**
 * @brief High-level rendering API with scene management.
 * 
 * Renderer provides a clean interface for submitting draw calls.
 * It follows the BeginScene/Submit/EndScene pattern.
 * 
 * Usage:
 *   Renderer::BeginScene(camera);
 *   Renderer::Submit(shader, mesh, transform);
 *   Renderer::EndScene();
 * 
 * The renderer automatically:
 * - Sets up view/projection matrices
 * - Binds shaders and meshes
 * - Manages render state
 * - Submits draw calls to RenderCommand
 */
class Renderer {
public:
    /**
     * @brief Initialize the renderer (must be called once at startup).
     */
    static void Init();

    /**
     * @brief Begin a new scene with the given camera.
     * @param camera Camera for view/projection matrices
     * 
     * Must be called before any Submit() calls.
     */
    static void BeginScene(const Camera& camera);

    /**
     * @brief Submit a mesh for rendering with a shader and transform.
     * @param shader Shader to use for rendering
     * @param mesh Mesh to render
     * @param transform Model transformation matrix
     * 
     * The renderer will:
     * 1. Bind the shader
     * 2. Set uniforms (MVP matrix, model matrix, etc.)
     * 3. Bind the mesh's vertex array
     * 4. Submit a draw call
     */
    static void Submit(const std::shared_ptr<Shader>& shader,
                      const std::shared_ptr<Mesh>& mesh,
                      const glm::mat4& transform = glm::mat4(1.0f));

    /**
     * @brief End the current scene.
     * 
     * Flushes any pending draw calls and resets state.
     */
    static void EndScene();

    /**
     * @brief Called when window is resized.
     * @param width New width in pixels
     * @param height New height in pixels
     */
    static void OnWindowResize(uint32_t width, uint32_t height);

private:
    struct SceneData {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* s_SceneData;
};

} // namespace Engine
