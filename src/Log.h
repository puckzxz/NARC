#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

class Log
{
public:
    static void Init();
    static std::shared_ptr<spdlog::logger>& Get();
private:
    Log()
    {
    }

    inline static std::shared_ptr<spdlog::logger> m_logger;
};

inline void Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_logger = spdlog::stdout_color_mt("NARC");
    m_logger->set_level(spdlog::level::debug);
}

inline std::shared_ptr<spdlog::logger>& Log::Get()
{
    return m_logger;
}

#define LOG_DEBUG(...) Log::Get()->debug(__VA_ARGS__)
#define LOG_ERROR(...) Log::Get()->error(__VA_ARGS__)
