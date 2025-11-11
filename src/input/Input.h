/**
 * @file Input.h
 * @brief Sistema de input unificado (teclado, ratón)
 * 
 * API estática para consultar estado de input en cualquier parte del motor.
 * Usa polling (consulta activa) en lugar de callbacks.
 * 
 * Uso:
 *   if (Input::IsKeyPressed(Key::W)) {
 *       camera.MoveForward(dt);
 *   }
 *   
 *   glm::vec2 mousePos = Input::GetMousePosition();
 */

#pragma once

#include <glm/glm.hpp>

namespace Engine {

/**
 * @enum Key
 * @brief Códigos de teclas (subset común de GLFW)
 */
enum class Key {
    // Números
    D0 = 48, D1 = 49, D2 = 50, D3 = 51, D4 = 52,
    D5 = 53, D6 = 54, D7 = 55, D8 = 56, D9 = 57,
    
    // Letras
    A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71, H = 72,
    I = 73, J = 74, K = 75, L = 76, M = 77, N = 78, O = 79, P = 80,
    Q = 81, R = 82, S = 83, T = 84, U = 85, V = 86, W = 87, X = 88,
    Y = 89, Z = 90,
    
    // Funciones
    F1 = 290, F2 = 291, F3 = 292, F4 = 293, F5 = 294, F6 = 295,
    F7 = 296, F8 = 297, F9 = 298, F10 = 299, F11 = 300, F12 = 301,
    
    // Modificadores
    LeftShift = 340, RightShift = 344,
    LeftControl = 341, RightControl = 345,
    LeftAlt = 342, RightAlt = 346,
    
    // Especiales
    Space = 32,
    Enter = 257,
    Escape = 256,
    Tab = 258,
    Backspace = 259,
    
    // Flechas
    Right = 262, Left = 263, Down = 264, Up = 265
};

/**
 * @enum MouseButton
 * @brief Botones del ratón
 */
enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2,
    Button4 = 3,
    Button5 = 4
};

/**
 * @class Input
 * @brief Sistema estático de consulta de input
 * 
 * Patrón: Static Class / Singleton implícito
 * Responsabilidad: Polling de teclado y ratón
 */
class Input {
public:
    /**
     * @brief Verifica si una tecla está presionada
     * @param key Código de tecla
     * @return true si está presionada
     */
    static bool IsKeyPressed(Key key);
    
    /**
     * @brief Verifica si un botón del ratón está presionado
     * @param button Botón del ratón
     * @return true si está presionado
     */
    static bool IsMouseButtonPressed(MouseButton button);
    
    /**
     * @brief Obtiene la posición del ratón en coordenadas de ventana
     * @return Vector2 con (x, y) desde esquina superior izquierda
     */
    static glm::vec2 GetMousePosition();
    
    /**
     * @brief Obtiene la posición X del ratón
     */
    static float GetMouseX();
    
    /**
     * @brief Obtiene la posición Y del ratón
     */
    static float GetMouseY();

private:
    // No instanciable (solo métodos estáticos)
    Input() = delete;
};

} // namespace Engine
