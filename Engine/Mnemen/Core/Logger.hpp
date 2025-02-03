//
// > Notice: Amélie Heinrich @ 2024
// > Create Time: 2025-02-03 11:57:47
//

#pragma once

#include <spdlog/spdlog.h>

#include "Common.hpp"

class Logger
{
public:
    static void Init();

    static Ref<spdlog::logger> GetLogger() { return sLogger; }
private:
    static Ref<spdlog::logger> sLogger;
};

#define LOG_TRACE(...)    ::Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)     ::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     ::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    ::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Logger::GetLogger()->critical(__VA_ARGS__)

#if defined(BEACHED_DEBUG)
    #define LOG_DEBUG(...)    ::Logger::GetLogger()->debug(__VA_ARGS__)
#else
    #define LOG_DEBUG(...)
#endif
