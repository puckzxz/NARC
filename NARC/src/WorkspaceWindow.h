#pragma once
#include <string>
#include "imgui.h"
#include "WorkspaceManager.h"

class WorkspaceWindow
{
public:
    void Draw();
    static WorkspaceWindow& Instance();
    void SetWorkspaces(const Workspaces& workspaces);
private:
    WorkspaceWindow();

    Workspaces m_workspaces;
    Workspace m_currentWorkspace;
};

inline WorkspaceWindow::WorkspaceWindow()
{
    m_workspaces = WorkspaceManager::Instance().GetWorkspaces();
    if (m_workspaces.workspaces.size() > 0)
    {
        m_currentWorkspace = m_workspaces.workspaces.front();
    }
}
