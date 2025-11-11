/**
 * @file Application.h
 * @brief Clase base para aplicaciones que usan el motor
 * 
 * Toda aplicación debe heredar de esta clase e implementar:
 * - OnInit(): Inicialización de recursos
 * - OnUpdate(dt): Lógica de actualización
 * - OnRender(): Renderizado
 * - OnShutdown(): Limpieza
 * 
 * Ejemplo:
 *   class MyGame : public Engine::Application {
 *       void OnInit() override { ... }
 *       void OnUpdate(float dt) override { ... }
 *       void OnRender() override { ... }
 *   };
 */

#pragma once

#include "core/Window.h"
#include <memory>

namespace Engine {

/**
 * @class Application
 * @brief Clase base abstracta para aplicaciones
 * 
 * Patrón: Template Method (define esqueleto del game loop)
 * Responsabilidad: Ciclo de vida de la aplicación
 */
class Application {
public:
    /**
     * @brief Constructor con propiedades de ventana
     */
    explicit Application(const WindowProps& props = WindowProps());
    
    /**
     * @brief Destructor virtual
     */
    virtual ~Application() = default;
    
    /**
     * @brief Ejecuta el game loop principal
     * Este método es el entry point de la aplicación
     */
    void Run();
    
    /**
     * @brief Solicita el cierre de la aplicación
     */
    void Close();
    
    /**
     * @brief Obtiene la ventana principal
     */
    Window& GetWindow() { return *m_window; }
    
    /**
     * @brief Obtiene la instancia de la aplicación (singleton)
     */
    static Application& Get() { return *s_instance; }

protected:
    /**
     * @brief Método de inicialización (override en clase derivada)
     * Se llama una vez al inicio
     */
    virtual void OnInit() {}
    
    /**
     * @brief Método de actualización (override en clase derivada)
     * Se llama cada frame antes de OnRender
     * @param deltaTime Tiempo transcurrido desde el último frame
     */
    virtual void OnUpdate([[maybe_unused]] float deltaTime) {}
    
    /**
     * @brief Método de renderizado (override en clase derivada)
     * Se llama cada frame después de OnUpdate
     */
    virtual void OnRender() {}
    
    /**
     * @brief Método de limpieza (override en clase derivada)
     * Se llama una vez antes de cerrar
     */
    virtual void OnShutdown() {}
    
    /**
     * @brief Callback cuando la ventana cambia de tamaño
     */
    virtual void OnWindowResize([[maybe_unused]] uint32_t width, [[maybe_unused]] uint32_t height) {}

private:
    std::unique_ptr<Window> m_window;
    bool m_running = true;
    
    static Application* s_instance;
};

} // namespace Engine
