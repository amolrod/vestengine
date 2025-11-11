/**
 * @file Time.h
 * @brief Sistema de gestión de tiempo del motor
 * 
 * Proporciona:
 * - Delta time entre frames
 * - Tiempo transcurrido desde inicio
 * - Cálculo de FPS
 * - Time scale (para slow-motion/fast-forward)
 * 
 * Uso:
 *   float dt = Time::DeltaTime();
 *   position += velocity * dt;
 */

#pragma once

#include <chrono>

namespace Engine {

/**
 * @class Time
 * @brief Gestor de tiempo del motor
 * 
 * Patrón: Singleton implícito (métodos estáticos)
 * Responsabilidad: Tracking de tiempo y FPS
 */
class Time {
public:
    /**
     * @brief Inicializa el sistema de tiempo
     * Debe llamarse al inicio del motor
     */
    static void Init();
    
    /**
     * @brief Actualiza el tiempo (llamar cada frame)
     * Calcula delta time y actualiza contadores
     */
    static void Update();
    
    /**
     * @brief Obtiene el delta time en segundos
     * @return Tiempo transcurrido desde el último frame
     */
    static float DeltaTime() { return s_deltaTime; }
    
    /**
     * @brief Obtiene el delta time sin escalar
     * @return Delta time real sin aplicar time scale
     */
    static float UnscaledDeltaTime() { return s_unscaledDeltaTime; }
    
    /**
     * @brief Obtiene el tiempo total desde el inicio en segundos
     */
    static float ElapsedTime() { return s_elapsedTime; }
    
    /**
     * @brief Obtiene el número de frame actual
     */
    static uint64_t FrameCount() { return s_frameCount; }
    
    /**
     * @brief Obtiene los FPS actuales
     * @return Frames por segundo (promediado)
     */
    static float FPS() { return s_fps; }
    
    /**
     * @brief Establece la escala de tiempo (1.0 = normal, 0.5 = mitad, 2.0 = doble)
     * @param scale Factor de escala (0.0 a cualquier valor positivo)
     */
    static void SetTimeScale(float scale) { s_timeScale = scale; }
    
    /**
     * @brief Obtiene la escala de tiempo actual
     */
    static float GetTimeScale() { return s_timeScale; }

private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    
    static TimePoint s_startTime;        // Momento de inicio del motor
    static TimePoint s_lastFrameTime;    // Momento del último frame
    
    static float s_deltaTime;            // Delta time escalado
    static float s_unscaledDeltaTime;    // Delta time sin escalar
    static float s_elapsedTime;          // Tiempo total transcurrido
    static float s_timeScale;            // Factor de escala temporal
    
    static float s_fps;                  // FPS calculados
    static float s_fpsUpdateTimer;       // Timer para actualizar FPS
    static int s_fpsFrameCount;          // Contador de frames para FPS
    
    static uint64_t s_frameCount;        // Número total de frames
};

} // namespace Engine
