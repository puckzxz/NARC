#pragma once
#include <string>
#include "imgui.h"
#include "WorkspaceManager.h"
#include <algorithm>

class WorkspaceWindow
{
public:
    void Draw();
    static WorkspaceWindow& Instance();
    void SetWorkspaces(const std::vector<Workspace>& workspaces);
private:
    WorkspaceWindow();

    std::vector<Workspace> m_workspaces;
    Workspace m_currentWorkspace;
};

inline WorkspaceWindow::WorkspaceWindow()
{
    m_workspaces = WorkspaceManager::Instance().GetWorkspaces();
    if (m_workspaces.size() > 0)
    {
        m_currentWorkspace = m_workspaces.front();
    }
}
