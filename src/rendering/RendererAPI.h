/**
 * @file RendererAPI.h
 * @brief Interfaz abstracta para la API de rendering
 * 
 * Define la interfaz que debe implementar cada backend (OpenGL, Vulkan, etc.)
 * Permite cambiar entre APIs sin modificar el código de alto nivel.
 */

#pragma once

#include "RenderingAPI.h"
// #include "VertexArray.h"  // TODO: Crear abstracción VertexArray
#include <glm/glm.hpp>
#include <memory>

namespace Engine {

// Forward declaration
class VertexArray;

/**
 * @class RendererAPI
 * @brief Clase base abstracta para todas las implementaciones de rendering
 * 
 * Cada plataforma (OpenGL, Vulkan) debe heredar de esta clase
 * e implementar todos los métodos virtuales puros.
 */
class RendererAPI {
public:
    virtual ~RendererAPI() = default;

    /**
     * @brief Inicializa la API de rendering
     * Configuraciones iniciales (depth test, blending, etc.)
     */
    virtual void Init() = 0;

    /**
     * @brief Cierra y limpia la API de rendering
     */
    virtual void Shutdown() = 0;

    /**
     * @brief Configura el viewport de rendering
     * @param x Coordenada X de inicio
     * @param y Coordenada Y de inicio
     * @param width Ancho del viewport
     * @param height Alto del viewport
     */
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    /**
     * @brief Establece el color de limpieza del buffer
     * @param color Color RGBA (0-1)
     */
    virtual void SetClearColor(const glm::vec4& color) = 0;

    /**
     * @brief Limpia los buffers (color, depth, stencil)
     */
    virtual void Clear() = 0;

    /**
     * @brief Dibuja geometría indexada
     * @param vertexArray VertexArray que contiene la geometría
     * @param indexCount Número de índices (0 = usar todos)
     */
    virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

    /**
     * @brief Dibuja geometría sin índices
     * @param vertexArray VertexArray que contiene la geometría
     * @param vertexCount Número de vértices
     */
    virtual void DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

    /**
     * @brief Habilita/deshabilita depth testing
     * @param enabled true para habilitar
     */
    virtual void SetDepthTest(bool enabled) = 0;

    /**
     * @brief Habilita/deshabilita blending
     * @param enabled true para habilitar
     */
    virtual void SetBlending(bool enabled) = 0;

    /**
     * @brief Habilita/deshabilita face culling
     * @param enabled true para habilitar
     */
    virtual void SetCulling(bool enabled) = 0;

    /**
     * @brief Obtiene la API actual
     * @return API de rendering en uso
     */
    static RenderingAPI GetAPI() { return s_API; }

    /**
     * @brief Establece la API a usar
     * @param api API de rendering
     * @note Debe llamarse ANTES de crear el RendererAPI
     */
    static void SetAPI(RenderingAPI api) { s_API = api; }

    /**
     * @brief Crea una instancia del RendererAPI según la API seleccionada
     * @return Puntero único a la implementación concreta
     */
    static std::unique_ptr<RendererAPI> Create();

private:
    static RenderingAPI s_API;
};

} // namespace Engine
