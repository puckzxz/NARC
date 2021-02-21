#pragma once
#include <string>
#include "imgui.h"
#include "WorkspaceManager.h"
#include <algorithm>

#include "App.h"
#include "Assert.h"

class WorkspaceWindow
{
public:
    void Draw();
    static WorkspaceWindow& Instance();
private:
    WorkspaceWindow();

    std::shared_ptr<const std::vector<Workspace>> m_workspaces;
    Workspace m_currentWorkspace;
    static inline std::vector m_invalidFolderNames = {
        "CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "LPT1",
        "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
    };
};

inline WorkspaceWindow::WorkspaceWindow()
{
    if (!WorkspaceManager::Instance().LoadWorkspaces())
    {
        NARC_ASSERT_NOT_REACHED("Failed to load workspaces");
    }
    m_workspaces = WorkspaceManager::Instance().Workspaces();
    if (m_workspaces->size() > 0)
    {
        m_currentWorkspace = m_workspaces->front();
    }
}
