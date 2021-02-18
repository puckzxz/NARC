#include "WorkspaceManager.h"

#include "Assert.h"

WorkspaceManager& WorkspaceManager::Instance()
{
    static WorkspaceManager it;
    return it;
}

bool WorkspaceManager::writeDefaultWorkspaceFile() const
{
    if (!std::filesystem::exists(m_folderName))
    {
        if (!std::filesystem::create_directory(m_folderName))
        {
            return false;
        }
    }
    std::vector<Request> reqs = {};
    const Workspace ws{"Default", reqs};
    try
    {
        std::ofstream os(formatWorkspaceFileName(ws.name));
        const json j = ws;
        os << j << std::endl;
        os.close();
    }
    catch (std::exception& e)
    {
        LOG_ERROR(e.what());
        return false;
    }
    return true;
}

void WorkspaceManager::SaveWorkspace(const Workspace& ws)
{
    for (auto& w : m_workspaces)
    {
        if (w.name == ws.name)
        {
            w = ws;
            saveWorkspace(formatWorkspaceFileName(ws.name), w);
        }
    }
}

std::vector<Workspace>& WorkspaceManager::GetWorkspaces()
{
    if (!std::filesystem::exists(formatWorkspaceFileName("Default")))
    {
        if (!writeDefaultWorkspaceFile())
        {
            NARC_ASSERT_NOT_REACHED("Failed to write workspace file");
        }
        return GetWorkspaces();
    }
    for (const auto& entry : std::filesystem::directory_iterator(m_folderName))
    {
        json j;
        std::ifstream is(entry.path());
        is >> j;
        is.close();
        m_workspaces.push_back(j.get<Workspace>());
    }
    return m_workspaces;
}

void WorkspaceManager::saveWorkspace(const std::string& fileName, const Workspace& ws)
{
    std::ofstream os(fileName);
    for (const auto& w : m_workspaces)
    {
        if (w.name == ws.name)
        {
            const json j = w;
            os << j << std::endl;
            os.close();
        }
    }
}

inline std::string WorkspaceManager::formatWorkspaceFileName(const std::string& name) const
{
    return m_folderName + "/" + name + ".narc";
}
