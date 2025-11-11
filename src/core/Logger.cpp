/**
 * @file Logger.cpp
 * @brief Implementación del sistema de logging
 */

#include "core/Logger.h"
#include <vector>

namespace Engine {

// Inicialización de miembros estáticos
std::shared_ptr<spdlog::logger> Logger::s_coreLogger;
std::shared_ptr<spdlog::logger> Logger::s_clientLogger;

void Logger::Init(const std::string& appName, bool enableFileLog) {
    // Vector de sinks (destinos de log)
    std::vector<spdlog::sink_ptr> logSinks;
    
    // Sink 1: Consola con colores
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern("%^[%T] %n: %v%$");
    logSinks.push_back(consoleSink);
    
    // Sink 2: Archivo (opcional)
    if (enableFileLog) {
        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/" + appName + ".log", true);
        fileSink->set_pattern("[%T] [%l] %n: %v");
        logSinks.push_back(fileSink);
    }
    
    // Crear logger del motor (core)
    s_coreLogger = std::make_shared<spdlog::logger>("MOTOR", begin(logSinks), end(logSinks));
    s_coreLogger->set_level(spdlog::level::trace);
    s_coreLogger->flush_on(spdlog::level::trace);
    spdlog::register_logger(s_coreLogger);
    
    // Crear logger de la aplicación cliente
    s_clientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
    s_clientLogger->set_level(spdlog::level::trace);
    s_clientLogger->flush_on(spdlog::level::trace);
    spdlog::register_logger(s_clientLogger);
    
    LOG_CORE_INFO("Sistema de logging inicializado");
    LOG_CORE_INFO("Aplicación: {}", appName);
    
    #ifdef DEBUG_BUILD
        LOG_CORE_DEBUG("Modo: DEBUG");
    #else
        LOG_CORE_INFO("Modo: RELEASE");
    #endif
    
    #ifdef PLATFORM_WINDOWS
        LOG_CORE_DEBUG("Plataforma: Windows");
    #elif defined(PLATFORM_MACOS)
        LOG_CORE_DEBUG("Plataforma: macOS");
    #elif defined(PLATFORM_LINUX)
        LOG_CORE_DEBUG("Plataforma: Linux");
    #endif
}

} // namespace Engine
