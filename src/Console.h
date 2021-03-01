#pragma once
#include <string>
#include <vector>
#include <fmt/core.h>

#include "Console.h"
#include "imgui.h"

class Console
{
public:
    template <typename... Args>
    static void Trace(const std::string& format, const Args&... args);
    template <typename... Args>
    static void Debug(const std::string& format, const Args&... args);
    template <typename... Args>
    static void Info(const std::string& format, const Args&... args);
    template <typename... Args>
    static void Warn(const std::string& format, const Args&... args);
    template <typename... Args>
    static void Error(const std::string& format, const Args&... args);
    template <typename... Args>
    static void Fatal(const std::string& format, const Args&... args);
    template <typename... Args>
    static void Log(const std::string& format, const Args&... args);

    static void Draw(const std::string& title, bool* open);
private:
    enum class LogLevel
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };

    struct Messages
    {
        LogLevel Level;
        std::string Message;
    };

    static inline std::vector<Messages> m_messages;
};

template <typename... Args>
void Console::Log(const std::string& format, const Args&... args)
{
    m_messages.push_back({
        LogLevel::Info,
        fmt::format(format, args...)
    });
}

template <typename... Args>
void Console::Debug(const std::string& format, const Args&... args)
{
    m_messages.push_back({
        LogLevel::Debug,
        fmt::format(format, args...)
    });
}

template <typename... Args>
void Console::Error(const std::string& format, const Args&... args)
{
    m_messages.push_back({
        LogLevel::Error,
        fmt::format(format, args...)
    });
}

template <typename... Args>
void Console::Trace(const std::string& format, const Args&... args)
{
    m_messages.push_back({
        LogLevel::Trace,
        fmt::format(format, args...)
    });
}

template <typename... Args>
void Console::Info(const std::string& format, const Args&... args)
{
    m_messages.push_back({
        LogLevel::Info,
        fmt::format(format, args...)
    });
}

template <typename... Args>
void Console::Warn(const std::string& format, const Args&... args)
{
    m_messages.push_back({
        LogLevel::Warn,
        fmt::format(format, args...)
    });
}

template <typename... Args>
void Console::Fatal(const std::string& format, const Args&... args)
{
    m_messages.push_back({
        LogLevel::Fatal,
        fmt::format(format, args...)
    });
}

inline void Console::Draw(const std::string& title, bool* open)
{
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title.c_str(), open))
    {
        ImGui::End();
        return;
    }
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Close Console"))
            *open = false;
        ImGui::EndPopup();
    }
    if (ImGui::Button("Clear"))
    {
        m_messages.clear();
    }
    ImGui::Separator();
    for (const auto& m : m_messages)
    {
        switch (m.Level)
        {
        case LogLevel::Trace:
            ImGui::TextColored(ImColor(200, 200, 200), m.Message.c_str());
            break;
        case LogLevel::Debug:
            ImGui::TextColored(ImColor(200, 200, 200), m.Message.c_str());
            break;;
        case LogLevel::Info:
            ImGui::TextColored(ImColor(255, 255, 255), m.Message.c_str());
            break;;
        case LogLevel::Warn:
            ImGui::TextColored(ImColor(255, 255, 0), m.Message.c_str());
            break;;
        case LogLevel::Error:
            ImGui::TextColored(ImColor(255, 0, 0), m.Message.c_str());
            break;;
        case LogLevel::Fatal:
            ImGui::TextColored(ImColor(255, 0, 0), m.Message.c_str());
            break;;
        default:
            ImGui::Text(m.Message.c_str());
            break;
        }
    }
    ImGui::End();
}
