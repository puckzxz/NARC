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
    WorkspaceWindow(){};

    Workspaces m_workspaces;
};
