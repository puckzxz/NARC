#include "SettingsManager.h"
#include "Console.h"

#include <fstream>
#include "Assert.h"

SettingsManager& SettingsManager::Instance()
{
    static SettingsManager it;
    return it;
}

void SettingsManager::SaveSettings(const Settings& s) const
{
    std::ofstream os(m_fileName);
    const json j = s;
    os << j << std::endl;
    os.close();
}

Settings SettingsManager::GetSettings() const
{
    if (!std::filesystem::exists(m_fileName))
    {
        if (!writeFile())
        {
            NARC_ASSERT_NOT_REACHED("Failed to create workspace file");
        }
        return GetSettings();
    }
    std::ifstream is(m_fileName);
    json j;
    is >> j;
    is.close();
    return j.get<Settings>();
}

bool SettingsManager::writeFile() const
{
    Settings s;
    s.windowWidth = 1280;
    s.windowHeight = 720;
    s.maximized = false;
    s.theme = AppTheme::Dark;
    std::ofstream os(m_fileName);
    const json j = s;
    os << j << std::endl;
    os.close();
    return true;
}
