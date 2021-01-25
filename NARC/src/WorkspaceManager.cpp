#include "WorkspaceManager.h"

#include <ranges>

WorkspaceManager& WorkspaceManager::Instance()
{
    static WorkspaceManager it;
    return it;
}

bool WorkspaceManager::WriteFile()
{
    std::vector<Request> reqs;
    for (int i = 1; i < 5; i++)
    {
        reqs.push_back({std::string ("Request" ) + std::to_string(i)});
    }
    const Workspace cf {"Test", reqs};
    Workspaces ws;
    ws.workspaces.push_back(cf);
    std::ofstream os("workspaces.narc");
    const json j = ws;
    os << j << std::endl;
    os.close();
    return true;
}

bool WorkspaceManager::DeleteFile()
{
    if (std::filesystem::exists("workspaces.narc"))
    {
        return std::filesystem::remove("workspaces.narc");
    }
    return true;
}

const Workspaces& WorkspaceManager::GetWorkspaces()
{
    if (!std::filesystem::exists("workspaces.narc"))
    {
        if (!WriteFile())
        {
            throw std::exception("Failed to create workspace file");
        }
        return GetWorkspaces();
    }
    std::ifstream is("workspaces.narc");
    json j;
    is >> j;
    is.close();
    const auto x = j.get<Workspaces>();
    for (const auto& w : x.workspaces)
    {
        std::cout << w.name << std::endl;
        for (const auto& z : w.requests)
        {
            std::cout << z.name << std::endl;
        }
    }
    return j.get<Workspaces>();
}
