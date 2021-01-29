#include "SettingsManager.h"

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
            throw std::exception("Failed to create workspace file");
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
    const auto s = Settings{1280, 720, false};
    std::ofstream os(m_fileName);
    const json j = s;
    os << j << std::endl;
    os.close();
    return true;
}
