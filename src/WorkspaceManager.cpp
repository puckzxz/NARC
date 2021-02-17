#include "WorkspaceManager.h"

#include "Assert.h"

WorkspaceManager& WorkspaceManager::Instance()
{
    static WorkspaceManager it;
    return it;
}

bool WorkspaceManager::writeDefaultWorkspaceFile()
{
    if (!std::filesystem::exists(m_folderName))
    {
        if (!std::filesystem::create_directory(m_folderName))
        {
            return false;
        }
    }
    std::vector<Request> reqs = {};
    const Workspace cf{"Default", reqs};
    Workspaces ws;
    ws.workspaces.push_back(cf);
    try
    {
        std::ofstream os(formatWorkspaceFileName(cf.name));
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
    for (auto& w : m_workspaces.workspaces)
    {
        if (w.name == ws.name)
        {
            w = ws;
        }
    }
    saveWorkspaces(formatWorkspaceFileName(ws.name));
}

Workspaces WorkspaceManager::GetWorkspaces()
{
    if (!std::filesystem::exists(formatWorkspaceFileName("Default")))
    {
        if (!writeDefaultWorkspaceFile())
        {
            NARC_ASSERT_NOT_REACHED("Failed to write workspace file");
        }
        return GetWorkspaces();
    }
    json j;
    for (const auto& entry : std::filesystem::directory_iterator(m_folderName))
    {
        std::cout << entry.path() << std::endl;
        std::ifstream is(entry.path());
        is >> j;
        is.close();
    }
    m_workspaces = j.get<Workspaces>();
    return m_workspaces;
}

void WorkspaceManager::saveWorkspaces(const std::string& name)
{
    std::ofstream os(name);
    const json j = m_workspaces;
    os << j << std::endl;
    os.close();
}

std::string WorkspaceManager::formatWorkspaceFileName(const std::string& name)
{
    return m_folderName + "/" + name + ".narc";
}
