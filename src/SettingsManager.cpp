#include "SettingsManager.h"
#include "Console.h"

#include <fstream>
#include "Assert.h"

static std::string m_fileName = "settings.narc";

static Settings currentSettings;

static bool writeDefaultSettingsFile()
{
    Settings s;
    s.windowWidth = 1280;
    s.windowHeight = 720;
    s.maximized = false;
    s.theme = AppTheme::Dark;
    try
    {
        std::ofstream os(m_fileName);
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
    if (!std::filesystem::exists(m_fileName))
    {
        if (!writeDefaultSettingsFile())
        {
            NARC_ASSERT_NOT_REACHED("Failed to create workspace file");
        }
    }
    std::ifstream is(m_fileName);
    json j;
    is >> j;
    is.close();
    currentSettings = j.get<Settings>();
}

void SettingsManager::SaveSettings(const Settings& s)
{
    std::ofstream os(m_fileName);
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
