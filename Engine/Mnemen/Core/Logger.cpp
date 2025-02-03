//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2024-12-03 05:51:34
//

#include <Core/Logger.hpp>

#include <vector>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

Ref<spdlog::logger> Logger::sLogger;

void Logger::Init()
{
    Vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(MakeRef<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(MakeRef<spdlog::sinks::basic_file_sink_mt>("mnemen.log", true));

    logSinks[0]->set_pattern("%^[%c] [%l] %v%$");
    logSinks[1]->set_pattern("[%c] [%l] %v");

    sLogger = MakeRef<spdlog::logger>("FUTABA", begin(logSinks), end(logSinks));
    spdlog::register_logger(sLogger);
    sLogger->set_level(spdlog::level::trace);
    sLogger->flush_on(spdlog::level::trace);
}