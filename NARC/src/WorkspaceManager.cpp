#include "WorkspaceManager.h"

WorkspaceManager& WorkspaceManager::Instance()
{
    static WorkspaceManager it;
    return it;
}

bool WorkspaceManager::WriteFile()
{
    std::vector<Request> reqs = {};
    const Workspace cf {"Default", reqs};
    Workspaces ws;
    ws.workspaces.push_back(cf);
    std::ofstream os(m_fileName);
    const json j = ws;
    os << j << std::endl;
    os.close();
    return true;
}



void WorkspaceManager::SaveWorkspace(const Workspace& ws)
{
    for (auto& w : m_workspaces.workspaces)
    {
        if (w.name == ws.name)
        {
            w.requests = ws.requests;
        }
    }
    saveWorkspaces();
}

Workspaces WorkspaceManager::GetWorkspaces()
{
    if (!std::filesystem::exists(m_fileName))
    {
        if (!WriteFile())
        {
            throw std::exception("Failed to create workspace file");
        }
        return GetWorkspaces();
    }
    std::ifstream is(m_fileName);
    json j;
    is >> j;
    is.close();
    m_workspaces = j.get<Workspaces>();
    return m_workspaces;
}

void WorkspaceManager::saveWorkspaces()
{
    std::ofstream os(m_fileName);
    const json j = m_workspaces;
    os << j << std::endl;
    os.close();
}
