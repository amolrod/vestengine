/**
 * @file Window.cpp
 * @brief Implementación de la gestión de ventanas
 */

#include "core/Window.h"
#include "core/Logger.h"

// En macOS, OpenGL está disponible directamente
#ifdef PLATFORM_MACOS
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#else
    #include <glad/gl.h>
#endif

// IMPORTANTE: GLFW_INCLUDE_NONE debe estar ANTES de incluir GLFW
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine {

// Contador de ventanas GLFW activas para gestión de init/terminate
static uint8_t s_glfwWindowCount = 0;

// Callback de error de GLFW
static void GLFWErrorCallback(int error, const char* description) {
    LOG_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

Window::Window(const WindowProps& props) {
    Init(props);
}

Window::~Window() {
    Shutdown();
}

void Window::Init(const WindowProps& props) {
    LOG_CORE_INFO("========================================");
    LOG_CORE_INFO("Window::Init - INICIO");
    LOG_CORE_INFO("========================================");
    
    m_data.title = props.title;
    m_data.width = props.width;
    m_data.height = props.height;
    m_data.vsync = props.vsync;
    
    LOG_CORE_INFO("Creando ventana: {0} ({1}x{2})", props.title, props.width, props.height);
    
    // Inicializar GLFW si es la primera ventana
    if (s_glfwWindowCount == 0) {
        LOG_CORE_INFO("[Window] Inicializando GLFW...");
        int success = glfwInit();
        if (!success) {
            LOG_CORE_CRITICAL("No se pudo inicializar GLFW!");
            return;
        }
        
        glfwSetErrorCallback(GLFWErrorCallback);
        LOG_CORE_INFO("GLFW inicializado correctamente");
    }
    
    // Configurar hints de GLFW para OpenGL
    LOG_CORE_INFO("[Window] Configurando hints de OpenGL...");
    #ifdef PLATFORM_MACOS
        // macOS solo soporta hasta OpenGL 4.1
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        LOG_CORE_DEBUG("Solicitando OpenGL 4.1 Core (macOS)");
    #else
        // Windows/Linux - Intentar 4.3 para mejor compatibilidad
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        LOG_CORE_DEBUG("Solicitando OpenGL 4.3 Core");
    #endif
    
    // Samples para MSAA (antialiasing)
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    // Crear ventana
    LOG_CORE_INFO("[Window] Creando ventana GLFW...");
    m_window = glfwCreateWindow(
        static_cast<int>(props.width),
        static_cast<int>(props.height),
        m_data.title.c_str(),
        props.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
        nullptr
    );
    
    if (!m_window) {
        LOG_CORE_CRITICAL("No se pudo crear la ventana GLFW!");
        glfwTerminate();
        return;
    }
    
    s_glfwWindowCount++;
    LOG_CORE_INFO("[Window] Ventana GLFW creada exitosamente");
    
    // Hacer el contexto OpenGL actual
    LOG_CORE_INFO("[Window] Haciendo contexto OpenGL actual...");
    glfwMakeContextCurrent(m_window);
    LOG_CORE_INFO("[Window] Contexto OpenGL configurado");
    
    // Inicializar cargador de OpenGL (solo en Windows/Linux)
    #ifndef PLATFORM_MACOS
        LOG_CORE_INFO("[Window] Inicializando GLAD...");
        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0) {
            LOG_CORE_CRITICAL("No se pudo inicializar GLAD!");
            return;
        }
        LOG_CORE_INFO("GLAD inicializado correctamente - OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    #else
        LOG_CORE_INFO("macOS: Usando OpenGL nativo (sin GLAD)");
    #endif
    
    // Información de OpenGL
    LOG_CORE_INFO("[Window] Obteniendo información de OpenGL...");
    LOG_CORE_INFO("OpenGL inicializado:");
    LOG_CORE_INFO("  Vendor:   {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    LOG_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    LOG_CORE_INFO("  Version:  {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    
    // Configurar VSync
    LOG_CORE_INFO("[Window] Configurando VSync...");
    SetVSync(m_data.vsync);
    LOG_CORE_INFO("[Window] VSync configurado");
    
    LOG_CORE_INFO("========================================");
    LOG_CORE_INFO("Window::Init - COMPLETADO");
    LOG_CORE_INFO("========================================");
    
    // Configurar viewport inicial
    glViewport(0, 0, m_data.width, m_data.height);
    
    // Guardar puntero a datos en la ventana GLFW (para callbacks)
    glfwSetWindowUserPointer(m_window, &m_data);
    
    // Configurar callbacks
    SetupCallbacks();
    
    LOG_CORE_INFO("Ventana creada exitosamente");
}

void Window::Shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        s_glfwWindowCount--;
        
        // Si no hay más ventanas, terminar GLFW
        if (s_glfwWindowCount == 0) {
            glfwTerminate();
            LOG_CORE_INFO("GLFW terminado");
        }
    }
}

void Window::SetupCallbacks() {
    // Callback de resize
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.width = width;
        data.height = height;
        
        glViewport(0, 0, width, height);
        
        if (data.resizeCallback) {
            data.resizeCallback();
        }
        
        LOG_CORE_DEBUG("Ventana redimensionada: {0}x{1}", width, height);
    });
    
    // Callback de cierre
    glfwSetWindowCloseCallback(m_window, []([[maybe_unused]] GLFWwindow* window) {
        LOG_CORE_INFO("Solicitud de cierre de ventana recibida");
    });
}

void Window::Update() {
    // Poll de eventos (teclado, ratón, etc.)
    glfwPollEvents();
    
    // Swap de buffers (double buffering)
    glfwSwapBuffers(m_window);
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::SetShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(m_window, shouldClose);
}

void Window::SetVSync(bool enabled) {
    if (enabled) {
        glfwSwapInterval(1);  // Activar VSync
    } else {
        glfwSwapInterval(0);  // Desactivar VSync
    }
    
    m_data.vsync = enabled;
    LOG_CORE_DEBUG("VSync: {0}", enabled ? "Activado" : "Desactivado");
}

} // namespace Engine
