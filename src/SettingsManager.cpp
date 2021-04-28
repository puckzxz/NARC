#include "SettingsManager.h"
#include "Console.h"

#include <fstream>
#include "Assert.h"
#include "TextEditor.h"
#include <map>

static std::string s_settingsFilename = "settings.narc";

static Settings currentSettings;

static bool writeDefaultSettingsFile()
{
    Settings s;
    s.windowWidth = 1280;
    s.windowHeight = 720;
    s.maximized = false;
    s.theme = AppTheme::Dark;
    s.palette = s_p;
    try
    {
        std::ofstream os(s_settingsFilename);
        const json j = s;
        os << j << std::endl;
        os.close();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR(e.what());
        return false;
    }
    return true;
}

static void loadSettings()
{
    if (!std::filesystem::exists(s_settingsFilename))
    {
        if (!writeDefaultSettingsFile())
        {
            NARC_ASSERT_NOT_REACHED("Failed to create workspace file");
        }
    }
    std::ifstream is(s_settingsFilename);
    json j;
    is >> j;
    is.close();
    try
    {
        currentSettings = j.get<Settings>();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR(e.what());
        LOG_ERROR("Failed to load settings file, using default settings");
        writeDefaultSettingsFile();
        loadSettings();
    }
}

void SettingsManager::SaveSettings(const Settings& s)
{
    std::ofstream os(s_settingsFilename);
    const json j = s;
    os << j << std::endl;
    os.close();
    loadSettings();
}

const Settings& SettingsManager::GetSettings()
{
    return currentSettings;
}

void SettingsManager::Init()
{
    loadSettings();
}

TextEditor::Palette SettingsManager::GetPalette()
{
    return currentSettings.palette;
}
