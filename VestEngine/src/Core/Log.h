#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Vest {

class Log {
public:
    static void Init();
    
    static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

}  // namespace Vest

// Core logging macros
#define VEST_CORE_TRACE(...)    ::Vest::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VEST_CORE_INFO(...)     ::Vest::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VEST_CORE_WARN(...)     ::Vest::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VEST_CORE_ERROR(...)    ::Vest::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VEST_CORE_CRITICAL(...) ::Vest::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client logging macros
#define VEST_TRACE(...)         ::Vest::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VEST_INFO(...)          ::Vest::Log::GetClientLogger()->info(__VA_ARGS__)
#define VEST_WARN(...)          ::Vest::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VEST_ERROR(...)         ::Vest::Log::GetClientLogger()->error(__VA_ARGS__)
#define VEST_CRITICAL(...)      ::Vest::Log::GetClientLogger()->critical(__VA_ARGS__)
