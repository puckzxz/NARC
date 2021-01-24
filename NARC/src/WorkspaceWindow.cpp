#include "WorkspaceWindow.h"

#include "WorkspaceManager.h"

WorkspaceWindow s_Workspace;

void WorkspaceWindow::Draw() const
{
    // TODO: Fix default item, refer to Request.cpp
    static std::string comboName = "Workspace 1";
    ImGui::Begin("Workspace");
    if(ImGui::BeginCombo("###Workspace", comboName.c_str()))
    {
        for (int i = 1; i < 6; i++)
        {
            std::string itemName = "Workspace " + std::to_string(i);
            if(ImGui::Selectable(itemName.c_str()))
            {
                comboName = itemName;
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::Button("Write File"))
    {
        WorkspaceManager::WriteFile();
    }
    if (ImGui::Button("Delete File"))
    {
        WorkspaceManager::DeleteFile();
    }
    if (ImGui::Button("Read File"))
    {
        WorkspaceManager::GetWorkspaces();
    }
    ImGui::End();
}

WorkspaceWindow& WorkspaceWindow::Get()
{
    return s_Workspace;
}
