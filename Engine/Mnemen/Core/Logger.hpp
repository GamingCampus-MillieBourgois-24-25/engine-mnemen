//
// > Notice: Amélie Heinrich @ 2025
// > Create Time: 2025-02-03 11:57:47
//

#pragma once

#include <spdlog/spdlog.h>
#include <imgui.h>

#include "Common.hpp"

/// @brief A Logger class for logging messages at various levels of severity.
/// 
/// This class provides static methods to log messages using the spdlog library. 
/// It supports different log levels, including trace, info, warn, error, and critical. 
/// Additionally, the class allows for conditional debug logging in debug builds.
class Logger
{
public:
    /// @brief An entry in the logger
    struct LogEntry
    {
        /// @brief The contents of the entry
        String Message;
        /// @brief The color of the entry
        ImVec4 Color;
    };

    /// @brief Initializes the logger.
    /// 
    /// This method sets up the logger instance and prepares it for use.
    static void Init();

    /// @brief Retrieves the logger instance.
    /// 
    /// This method returns a shared pointer to the logger instance, which can be used to log messages.
    /// @return A shared pointer to the logger instance.
    static Ref<spdlog::logger> GetLogger() { return sLogger; }

    /// @brief The vector containing the log data. Make sure to clear it every once in a while! ;)
    static Vector<LogEntry> sEntries;
private:
    /// @brief The shared pointer to the logger instance.
    static Ref<spdlog::logger> sLogger;
};

/// @brief Macro for logging trace-level messages.
/// 
/// Logs a message with the "trace" log level using the Logger class.
/// @param ... The message to log.
#define LOG_TRACE(...)    ::Logger::GetLogger()->trace(__VA_ARGS__)

/// @brief Macro for logging info-level messages.
/// 
/// Logs a message with the "info" log level using the Logger class.
/// @param ... The message to log.
#define LOG_INFO(...)     ::Logger::GetLogger()->info(__VA_ARGS__)

/// @brief Macro for logging warn-level messages.
/// 
/// Logs a message with the "warn" log level using the Logger class.
/// @param ... The message to log.
#define LOG_WARN(...)     ::Logger::GetLogger()->warn(__VA_ARGS__)

/// @brief Macro for logging error-level messages.
/// 
/// Logs a message with the "error" log level using the Logger class.
/// @param ... The message to log.
#define LOG_ERROR(...)    ::Logger::GetLogger()->error(__VA_ARGS__)

/// @brief Macro for logging critical-level messages.
/// 
/// Logs a message with the "critical" log level using the Logger class.
/// @param ... The message to log.
#define LOG_CRITICAL(...) ::Logger::GetLogger()->critical(__VA_ARGS__)

/// @brief Macro for logging debug-level messages (enabled in debug builds only).
/// 
/// Logs a message with the "debug" log level using the Logger class. This is only available
/// in debug builds, and the macro will be empty in release builds.
/// @param ... The message to log.
#if defined(MNEMEN_DEBUG)
    #define LOG_DEBUG(...)    ::Logger::GetLogger()->debug(__VA_ARGS__)
#else
    #define LOG_DEBUG(...)
#endif
