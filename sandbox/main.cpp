/**
 * @file main.cpp
 * @brief Sandbox - Aplicación de prueba del Motor Gráfico 3D
 * 
 * Esta aplicación demuestra las capacidades básicas del motor:
 * - Creación de ventana
 * - Game loop
 * - Sistema de input
 * - Logging
 * - Time management
 */

#include <core/Application.h>
#include <core/Logger.h>
#include <core/Time.h>
#include <input/Input.h>
#include <core/EntryPoint.h>

// En macOS, OpenGL está disponible directamente
#ifdef PLATFORM_MACOS
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_NONE
    #include <OpenGL/gl3.h>
#else
    #include <glad/glad.h>
#endif

#include <glm/glm.hpp>

/**
 * @class SandboxApp
 * @brief Aplicación de demostración
 */
class SandboxApp : public Engine::Application {
public:
    SandboxApp() : Engine::Application(Engine::WindowProps("Sandbox - Motor Gráfico 3D", 1280, 720)) {
        LOG_INFO("Constructor de SandboxApp");
    }
    
    ~SandboxApp() override {
        LOG_INFO("Destructor de SandboxApp");
    }
    
    void OnInit() override {
        LOG_INFO("╔══════════════════════════════════════════════╗");
        LOG_INFO("║           SANDBOX INICIALIZADO               ║");
        LOG_INFO("╚══════════════════════════════════════════════╝");
        LOG_INFO("");
        LOG_INFO("Controles:");
        LOG_INFO("  ESC       - Cerrar aplicación");
        LOG_INFO("  1,2,3,4   - Cambiar color de fondo");
        LOG_INFO("  SPACE     - Color aleatorio");
        LOG_INFO("  F1        - Toggle VSync");
        LOG_INFO("  Mouse     - Visualizar posición en consola");
        LOG_INFO("");
        
        // Configuración inicial de OpenGL
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        
        m_backgroundColor = glm::vec3(0.1f, 0.1f, 0.15f);
    }
    
    void OnUpdate(float deltaTime) override {
        // Input: Cerrar con ESC
        if (Engine::Input::IsKeyPressed(Engine::Key::Escape)) {
            LOG_INFO("ESC presionado - Cerrando aplicación");
            Close();
        }
        
        // Input: Cambiar colores de fondo
        if (Engine::Input::IsKeyPressed(Engine::Key::D1)) {
            m_backgroundColor = glm::vec3(0.2f, 0.3f, 0.5f);  // Azul
            LOG_INFO("Color: Azul");
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D2)) {
            m_backgroundColor = glm::vec3(0.3f, 0.5f, 0.3f);  // Verde
            LOG_INFO("Color: Verde");
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D3)) {
            m_backgroundColor = glm::vec3(0.5f, 0.3f, 0.3f);  // Rojo
            LOG_INFO("Color: Rojo");
        }
        if (Engine::Input::IsKeyPressed(Engine::Key::D4)) {
            m_backgroundColor = glm::vec3(0.1f, 0.1f, 0.15f); // Gris oscuro
            LOG_INFO("Color: Gris oscuro");
        }
        
        // Input: Color aleatorio con SPACE
        static bool spaceWasPressed = false;
        bool spacePressed = Engine::Input::IsKeyPressed(Engine::Key::Space);
        if (spacePressed && !spaceWasPressed) {
            m_backgroundColor = glm::vec3(
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX
            );
            LOG_INFO("Color aleatorio: ({:.2f}, {:.2f}, {:.2f})", 
                m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b);
        }
        spaceWasPressed = spacePressed;
        
        // Input: Toggle VSync con F1
        static bool f1WasPressed = false;
        bool f1Pressed = Engine::Input::IsKeyPressed(Engine::Key::F1);
        if (f1Pressed && !f1WasPressed) {
            bool currentVSync = GetWindow().IsVSync();
            GetWindow().SetVSync(!currentVSync);
            LOG_INFO("VSync: {}", !currentVSync ? "ON" : "OFF");
        }
        f1WasPressed = f1Pressed;
        
        // Mostrar posición del mouse (cada 1 segundo)
        static float mouseLogTimer = 0.0f;
        mouseLogTimer += deltaTime;
        if (mouseLogTimer >= 1.0f) {
            if (Engine::Input::IsMouseButtonPressed(Engine::MouseButton::Left)) {
                glm::vec2 mousePos = Engine::Input::GetMousePosition();
                LOG_DEBUG("Mouse: ({:.0f}, {:.0f}) | LMB presionado", mousePos.x, mousePos.y);
            }
            mouseLogTimer = 0.0f;
        }
        
        // Animación de color (pulsación suave)
        m_colorPulse += deltaTime * 0.5f;
        float pulse = (glm::sin(m_colorPulse) + 1.0f) * 0.5f;  // 0.0 a 1.0
        
        glm::vec3 finalColor = m_backgroundColor * (0.8f + pulse * 0.2f);
        glClearColor(finalColor.r, finalColor.g, finalColor.b, 1.0f);
    }
    
    void OnRender() override {
        // Por ahora solo limpiamos la pantalla (ya se hace en Application)
        // En la FASE 2 renderizaremos geometría aquí
    }
    
    void OnShutdown() override {
        LOG_INFO("Limpieza de SandboxApp");
    }
    
    void OnWindowResize(uint32_t width, uint32_t height) override {
        LOG_INFO("Ventana redimensionada: {}x{}", width, height);
    }

private:
    glm::vec3 m_backgroundColor;
    float m_colorPulse = 0.0f;
};

/**
 * @brief Función de creación de aplicación
 * Requerida por EntryPoint.h
 */
Engine::Application* CreateApplication() {
    return new SandboxApp();
}
