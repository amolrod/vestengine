#include "Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Vest {

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
    std::vector<spdlog::sink_ptr> logSinks;
    
    // Console sink with color
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    
    // File sink
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("VestEngine.log", true));
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");
    
    // Create loggers
    s_CoreLogger = std::make_shared<spdlog::logger>("VEST", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_CoreLogger);
    s_CoreLogger->set_level(spdlog::level::trace);
    s_CoreLogger->flush_on(spdlog::level::trace);
    
    s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_ClientLogger);
    s_ClientLogger->set_level(spdlog::level::trace);
    s_ClientLogger->flush_on(spdlog::level::trace);
    
    VEST_CORE_INFO("VestEngine logging initialized");
}

}  // namespace Vest
