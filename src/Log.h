#pragma once
#include <memory>
#include <spdlog/spdlog.h>

class Log
{
public:
    static void Init();
    static std::shared_ptr<spdlog::logger>& Get();
private:
    Log(){};
    inline static std::shared_ptr<spdlog::logger> m_logger;
};

#define LOG_DEBUG(...) Log::Get()->debug(__VA_ARGS__)
#define LOG_ERROR(...) Log::Get()->error(__VA_ARGS__)
