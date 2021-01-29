#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

struct Settings
{
    int32_t windowWidth;
    int32_t windowHeight;
    bool maximized;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, windowWidth, windowHeight, maximized);
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
