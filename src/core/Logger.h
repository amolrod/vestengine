/**
 * @file Logger.h
 * @brief Sistema de logging centralizado usando spdlog
 * 
 * Este módulo proporciona macros convenientes para logging en todo el motor.
 * Características:
 * - Niveles de log: TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL
 * - Colored console output
 * - Thread-safe
 * - Archivos de log opcionales
 * 
 * Uso:
 *   LOG_INFO("Iniciando motor versión {}", version);
 *   LOG_ERROR("Error al cargar textura: {}", filename);
 */

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>

namespace Engine {

/**
 * @class Logger
 * @brief Wrapper de spdlog para el motor
 * 
 * Patrón: Singleton (acceso estático)
 * Responsabilidad: Inicializar y gestionar loggers
 */
class Logger {
public:
    /**
     * @brief Inicializa el sistema de logging
     * @param appName Nombre de la aplicación para el log
     * @param enableFileLog Si true, crea archivo de log
     */
    static void Init(const std::string& appName = "MotorGrafico", bool enableFileLog = false);
    
    /**
     * @brief Obtiene el logger principal del motor
     */
    static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
    
    /**
     * @brief Obtiene el logger de la aplicación cliente
     */
    static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_coreLogger;
    static std::shared_ptr<spdlog::logger> s_clientLogger;
};

} // namespace Engine

// =============================================================================
// Macros convenientes para logging
// =============================================================================

// Core logger (para código interno del motor)
#define LOG_CORE_TRACE(...)    ::Engine::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_DEBUG(...)    ::Engine::Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define LOG_CORE_INFO(...)     ::Engine::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)     ::Engine::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...)    ::Engine::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...) ::Engine::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client logger (para código de usuario/aplicación)
#define LOG_TRACE(...)         ::Engine::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)         ::Engine::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)          ::Engine::Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::Engine::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::Engine::Logger::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::Engine::Logger::GetClientLogger()->critical(__VA_ARGS__)

// Macros condicionales para debug (se compilan solo en Debug)
#ifdef DEBUG_BUILD
    #define LOG_DEBUG_ONLY(...)    LOG_DEBUG(__VA_ARGS__)
    #define LOG_CORE_DEBUG_ONLY(...) LOG_CORE_DEBUG(__VA_ARGS__)
#else
    #define LOG_DEBUG_ONLY(...)
    #define LOG_CORE_DEBUG_ONLY(...)
#endif
