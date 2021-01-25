#pragma once
#include <string>
#include "imgui.h"
#include "WorkspaceManager.h"

class WorkspaceWindow
{
public:
    void Draw() const;
    static WorkspaceWindow& Instance();
private:
    WorkspaceWindow(){};

    Workspaces m_workspaces;
};
