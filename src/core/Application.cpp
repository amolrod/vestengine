/**
 * @file Application.cpp
 * @brief Implementación del game loop principal
 */

#include "core/Application.h"
#include "core/Logger.h"
#include "core/Time.h"
#include "input/Input.h"

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

// Variable global para acceso desde Input (temporal, mejorar después)
GLFWwindow* g_currentWindow = nullptr;

namespace Engine {

Application* Application::s_instance = nullptr;

Application::Application(const WindowProps& props) {
    // Singleton
    if (s_instance != nullptr) {
        fprintf(stderr, "ERROR CRITICO: ¡La aplicación ya existe! Solo puede haber una instancia.\n");
        return;
    }
    s_instance = this;
    
    // Inicializar sistemas core
    Logger::Init(props.title);
    
    LOG_CORE_INFO("========================================");
    LOG_CORE_INFO("Application::Application - INICIO");
    LOG_CORE_INFO("========================================");
    LOG_CORE_INFO("[Application] Logger inicializado OK");
    
    LOG_CORE_INFO("[Application] Inicializando Time...");
    Time::Init();
    LOG_CORE_INFO("[Application] Time inicializado OK");
    
    // Crear ventana
    LOG_CORE_INFO("[Application] Creando ventana...");
    m_window = std::make_unique<Window>(props);
    LOG_CORE_INFO("[Application] Ventana creada OK");
    
    LOG_CORE_INFO("[Application] Configurando callback de ventana...");
    g_currentWindow = m_window->GetNativeWindow();
    
    // Configurar callback de resize
    m_window->SetResizeCallback([this]() {
        OnWindowResize(m_window->GetWidth(), m_window->GetHeight());
    });
    LOG_CORE_INFO("[Application] Callbacks configurados OK");
    
    LOG_CORE_INFO("========================================");
    LOG_CORE_INFO("Application::Application - COMPLETADO");
    LOG_CORE_INFO("========================================");
    
    LOG_CORE_INFO("Aplicación inicializada correctamente");
}

void Application::Run() {
    LOG_CORE_INFO("=== Iniciando Game Loop ===");
    
    // Llamar a inicialización de usuario
    OnInit();
    
    // Game Loop principal
    while (m_running && !m_window->ShouldClose()) {
        // Actualizar tiempo
        Time::Update();
        float deltaTime = Time::DeltaTime();
        
        // Lógica de usuario
        OnUpdate(deltaTime);
        
        // Renderizado
        // Clear screen (fondo negro por defecto)
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        OnRender();
        
        // Actualizar ventana (swap buffers y poll events)
        m_window->Update();
        
        // Log de FPS cada cierto tiempo (solo en debug)
        #ifdef DEBUG_BUILD
        static float fpsTimer = 0.0f;
        fpsTimer += deltaTime;
        if (fpsTimer >= 2.0f) {
            LOG_CORE_DEBUG_ONLY("FPS: {:.1f} | Frame: {} | DT: {:.3f}ms", 
                Time::FPS(), Time::FrameCount(), deltaTime * 1000.0f);
            fpsTimer = 0.0f;
        }
        #endif
    }
    
    LOG_CORE_INFO("=== Finalizando Game Loop ===");
    
    // Llamar a limpieza de usuario
    OnShutdown();
    
    LOG_CORE_INFO("Aplicación cerrada correctamente");
}

void Application::Close() {
    LOG_CORE_INFO("Cerrando aplicación...");
    m_running = false;
}

} // namespace Engine
