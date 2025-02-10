//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-12-03 05:51:34
//

#include <Core/Logger.hpp>

#include <vector>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

Ref<spdlog::logger> Logger::sLogger;
Vector<Logger::LogEntry> Logger::sEntries;

ImVec4 LevelToColor(spdlog::level::level_enum level)
{
    switch (level) {
        case spdlog::level::level_enum::info:
            return ImVec4(0, 1, 0, 1);
        case spdlog::level::level_enum::err:
            return ImVec4(1, 0, 0, 1);
        case spdlog::level::level_enum::warn:
            return ImVec4(0, 1, 1, 1);
    }
    return ImVec4(1, 1, 1, 1);
}

class VectorSink : public spdlog::sinks::sink
{
public:
    void log(const spdlog::details::log_msg& msg) override {
        // Convert time_point to time_t for easier formatting
        auto time_point = std::chrono::system_clock::to_time_t(msg.time);
        std::tm tm = *std::localtime(&time_point);

        // Format the timestamp
        std::stringstream time_stream;
        time_stream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

        // Convert the log message to a string and store it in the vector
        String log_message = fmt::format("{} [{}] {}", time_stream.str(), spdlog::level::to_string_view(msg.level), msg.payload);
        Logger::sEntries.push_back({ log_message, LevelToColor(msg.level) });
    }

    void flush() override {
        Logger::sEntries.clear();
    }

    void set_pattern(const std::string &pattern) override {}
    void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override {}
};

void Logger::Init()
{
    Vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(MakeRef<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(MakeRef<spdlog::sinks::basic_file_sink_mt>("mnemen.log", true));
    logSinks.emplace_back(MakeRef<VectorSink>());

    logSinks[0]->set_pattern("%^[%c] [%l] %v%$");
    logSinks[1]->set_pattern("[%c] [%l] %v");

    sLogger = MakeRef<spdlog::logger>("MNEMEN", begin(logSinks), end(logSinks));
    spdlog::register_logger(sLogger);
    sLogger->set_level(spdlog::level::trace);
    sLogger->flush_on(spdlog::level::trace);
}