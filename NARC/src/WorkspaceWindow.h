#pragma once
#include <string>
#include "imgui.h"

// TODO: Convert to singleton pattern

class WorkspaceWindow
{
public:
    void Draw() const;
    static WorkspaceWindow& Get();
};
