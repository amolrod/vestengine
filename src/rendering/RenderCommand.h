#pragma once

// macOS requires native OpenGL headers
#ifdef PLATFORM_MACOS
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#else
    #include <glad/gl.h>
#endif

#include <glm/glm.hpp>
#include <cstdint>

namespace Engine {

/**
 * @brief Static API for low-level OpenGL rendering commands.
 * 
 * RenderCommand provides a thin abstraction over OpenGL state management
 * and drawing commands. All methods are static for global access.
 * 
 * Usage:
 *   RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
 *   RenderCommand::Clear();
 *   RenderCommand::DrawIndexed(vertexArray, indexCount);
 */
class RenderCommand {
public:
    /**
     * @brief Initialize the rendering API (OpenGL state, extensions, etc.)
     */
    static void Init();

    /**
     * @brief Set the viewport dimensions.
     * @param x X coordinate of lower-left corner
     * @param y Y coordinate of lower-left corner
     * @param width Viewport width in pixels
     * @param height Viewport height in pixels
     */
    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    /**
     * @brief Set the clear color for glClear().
     * @param color RGBA color (values 0-1)
     */
    static void SetClearColor(const glm::vec4& color);

    /**
     * @brief Clear the screen buffers.
     * Clears GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
     */
    static void Clear();

    /**
     * @brief Draw indexed geometry (triangles).
     * @param indexCount Number of indices to draw (from currently bound IBO)
     */
    static void DrawIndexed(uint32_t indexCount);

    /**
     * @brief Enable depth testing.
     */
    static void EnableDepthTest();

    /**
     * @brief Disable depth testing.
     */
    static void DisableDepthTest();

    /**
     * @brief Enable backface culling.
     */
    static void EnableCulling();

    /**
     * @brief Disable backface culling.
     */
    static void DisableCulling();

    /**
     * @brief Enable blending (for transparency).
     */
    static void EnableBlending();

    /**
     * @brief Disable blending.
     */
    static void DisableBlending();

    /**
     * @brief Set blend function for transparency.
     * @param src Source blend factor (default: GL_SRC_ALPHA)
     * @param dst Destination blend factor (default: GL_ONE_MINUS_SRC_ALPHA)
     */
    static void SetBlendFunc(uint32_t src, uint32_t dst);
};

} // namespace Engine
