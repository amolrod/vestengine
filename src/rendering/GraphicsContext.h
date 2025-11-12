/**
 * @file GraphicsContext.h
 * @brief Contexto de rendering abstracto para cada ventana
 * 
 * Gestiona la inicialización del contexto gráfico y swap de buffers.
 * Cada API (OpenGL, Vulkan) tiene su propia implementación.
 */

#pragma once

#include <memory>

namespace Engine {

/**
 * @class GraphicsContext
 * @brief Interfaz abstracta para contextos de rendering
 * 
 * Cada ventana tiene su propio GraphicsContext que gestiona:
 * - Inicialización de la API (GLAD para OpenGL, surface para Vulkan)
 * - Swap de buffers (presentación de frames)
 * - Configuración específica de la ventana
 */
class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;

    /**
     * @brief Inicializa el contexto de rendering
     * Para OpenGL: carga GLAD, configura debug callback
     * Para Vulkan: crea surface, swapchain
     */
    virtual void Init() = 0;

    /**
     * @brief Intercambia los buffers (presenta el frame)
     * Para OpenGL: glfwSwapBuffers()
     * Para Vulkan: vkQueuePresentKHR()
     */
    virtual void SwapBuffers() = 0;

    /**
     * @brief Crea un contexto según la API activa
     * @param window Puntero nativo a la ventana (GLFWwindow*)
     * @return Contexto de rendering
     */
    static std::unique_ptr<GraphicsContext> Create(void* window);
};

} // namespace Engine
