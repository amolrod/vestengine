/**
 * @file Time.cpp
 * @brief Implementación del sistema de tiempo
 */

#include "core/Time.h"
#include "core/Logger.h"

namespace Engine {

// Inicialización de miembros estáticos
Time::TimePoint Time::s_startTime;
Time::TimePoint Time::s_lastFrameTime;

float Time::s_deltaTime = 0.0f;
float Time::s_unscaledDeltaTime = 0.0f;
float Time::s_elapsedTime = 0.0f;
float Time::s_timeScale = 1.0f;

float Time::s_fps = 0.0f;
float Time::s_fpsUpdateTimer = 0.0f;
int Time::s_fpsFrameCount = 0;

uint64_t Time::s_frameCount = 0;

void Time::Init() {
    s_startTime = Clock::now();
    s_lastFrameTime = s_startTime;
    
    s_deltaTime = 0.0f;
    s_unscaledDeltaTime = 0.0f;
    s_elapsedTime = 0.0f;
    s_timeScale = 1.0f;
    
    s_fps = 0.0f;
    s_fpsUpdateTimer = 0.0f;
    s_fpsFrameCount = 0;
    
    s_frameCount = 0;
    
    LOG_CORE_INFO("Sistema de tiempo inicializado");
}

void Time::Update() {
    // Calcular tiempo actual
    TimePoint currentTime = Clock::now();
    
    // Calcular delta time en segundos
    std::chrono::duration<float> delta = currentTime - s_lastFrameTime;
    s_unscaledDeltaTime = delta.count();
    
    // Aplicar time scale
    s_deltaTime = s_unscaledDeltaTime * s_timeScale;
    
    // Actualizar tiempo total
    std::chrono::duration<float> elapsed = currentTime - s_startTime;
    s_elapsedTime = elapsed.count();
    
    // Actualizar contador de frames
    s_frameCount++;
    
    // Calcular FPS (actualizamos cada 0.5 segundos para suavizado)
    s_fpsUpdateTimer += s_unscaledDeltaTime;
    s_fpsFrameCount++;
    
    if (s_fpsUpdateTimer >= 0.5f) {
        s_fps = static_cast<float>(s_fpsFrameCount) / s_fpsUpdateTimer;
        s_fpsUpdateTimer = 0.0f;
        s_fpsFrameCount = 0;
    }
    
    // Guardar tiempo para el próximo frame
    s_lastFrameTime = currentTime;
    
    // Protección contra delta times muy grandes (pausa larga o debugging)
    if (s_deltaTime > 0.1f) {
        LOG_CORE_WARN("Delta time muy grande detectado: {:.3f}s (limitado a 0.1s)", s_deltaTime);
        s_deltaTime = 0.1f;
    }
}

} // namespace Engine
