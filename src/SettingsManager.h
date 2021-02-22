#pragma once

#include <filesystem>
#include <string>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

enum AppTheme
{
    Dark,
    Light,
    Classic
};

struct Settings
{
    int32_t windowWidth;
    int32_t windowHeight;
    bool maximized;
    AppTheme theme;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, windowWidth, windowHeight, maximized, theme);
};

class SettingsManager
{
public:
    static SettingsManager& Instance();
    void SaveSettings(const Settings& s) const;
    Settings GetSettings() const;
private:
    SettingsManager(){};
    bool writeFile() const;
    std::string m_fileName = "settings.narc";
};
