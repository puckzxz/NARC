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
    static void SaveSettings(const Settings& s);
    static const Settings& GetSettings();
    static void Init();
private:
    SettingsManager() {};
};
