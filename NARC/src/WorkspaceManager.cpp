#include "WorkspaceManager.h"

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
        reqs.push_back({
                std::string("Request ") + std::to_string(i),
                std::string("GET"),
                std::string("https://reqres.in/api/users/" + std::to_string(i))
            });
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

Workspaces WorkspaceManager::GetWorkspaces() const
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
    return j.get<Workspaces>();
}
