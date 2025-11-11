/**
 * @file Window.h
 * @brief Abstracción de ventana multiplataforma usando GLFW
 * 
 * Encapsula la creación y gestión de ventanas con contexto OpenGL.
 * Características:
 * - Creación de ventana con contexto OpenGL
 * - Gestión de eventos básicos
 * - VSync configurable
 * - Callbacks para resize, close, etc.
 */

#pragma once

#include <string>
#include <functional>
#include <cstdint>

// Forward declaration para evitar incluir GLFW en el header
struct GLFWwindow;

namespace Engine {

/**
 * @struct WindowProps
 * @brief Propiedades de configuración de una ventana
 */
struct WindowProps {
    std::string title;
    uint32_t width;
    uint32_t height;
    bool vsync;
    bool fullscreen;
    
    WindowProps(
        const std::string& title = "Motor Gráfico 3D",
        uint32_t width = 1280,
        uint32_t height = 720,
        bool vsync = true,
        bool fullscreen = false
    ) : title(title), width(width), height(height), vsync(vsync), fullscreen(fullscreen) {}
};

/**
 * @class Window
 * @brief Gestión de ventana y contexto OpenGL
 * 
 * Patrón: Facade (oculta complejidad de GLFW)
 * Responsabilidad: Ciclo de vida de ventana, eventos, contexto OpenGL
 */
class Window {
public:
    using EventCallbackFn = std::function<void()>;
    
    /**
     * @brief Constructor con propiedades
     */
    explicit Window(const WindowProps& props = WindowProps());
    
    /**
     * @brief Destructor (limpia recursos GLFW)
     */
    ~Window();
    
    // No permitir copia (ventana es recurso único)
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    
    /**
     * @brief Actualiza la ventana (poll events y swap buffers)
     * Debe llamarse cada frame
     */
    void Update();
    
    /**
     * @brief Verifica si la ventana debe cerrarse
     */
    bool ShouldClose() const;
    
    /**
     * @brief Establece si la ventana debe cerrarse
     */
    void SetShouldClose(bool shouldClose);
    
    /**
     * @brief Obtiene el ancho de la ventana
     */
    uint32_t GetWidth() const { return m_data.width; }
    
    /**
     * @brief Obtiene el alto de la ventana
     */
    uint32_t GetHeight() const { return m_data.height; }
    
    /**
     * @brief Obtiene el aspect ratio (width/height)
     */
    float GetAspectRatio() const { 
        return static_cast<float>(m_data.width) / static_cast<float>(m_data.height); 
    }
    
    /**
     * @brief Activa/desactiva VSync
     */
    void SetVSync(bool enabled);
    
    /**
     * @brief Verifica si VSync está activo
     */
    bool IsVSync() const { return m_data.vsync; }
    
    /**
     * @brief Obtiene el puntero nativo a GLFWwindow
     * Útil para integraciones directas con GLFW
     */
    GLFWwindow* GetNativeWindow() const { return m_window; }
    
    /**
     * @brief Establece callback de resize
     */
    void SetResizeCallback(EventCallbackFn callback) { 
        m_data.resizeCallback = callback; 
    }

private:
    /**
     * @brief Inicializa GLFW y crea la ventana
     */
    void Init(const WindowProps& props);
    
    /**
     * @brief Limpia recursos GLFW
     */
    void Shutdown();
    
    /**
     * @brief Configura callbacks de eventos GLFW
     */
    void SetupCallbacks();

private:
    GLFWwindow* m_window;
    
    // Estructura de datos que GLFW puede acceder via pointer
    struct WindowData {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool vsync;
        
        EventCallbackFn resizeCallback;
    };
    
    WindowData m_data;
};

} // namespace Engine
