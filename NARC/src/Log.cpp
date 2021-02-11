#include "Log.h"

#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

void Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_logger = spdlog::stdout_color_mt("NARC");
    m_logger->set_level(spdlog::level::debug);
}

std::shared_ptr<spdlog::logger> Log::Get()
{
    return m_logger;
}
