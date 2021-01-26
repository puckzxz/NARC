﻿#include "WorkspaceWindow.h"

#include "WorkspaceManager.h"


void WorkspaceWindow::Draw()
{
    // TODO: Fix default item, refer to Request.cpp
    static std::string comboName = "Workspace 1";
    ImGui::Begin("Workspace");
    if (ImGui::BeginCombo("###Workspace", comboName.c_str()))
    {
        for (int i = 1; i < 6; i++)
        {
            std::string itemName = "Workspace " + std::to_string(i);
            if (ImGui::Selectable(itemName.c_str()))
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
    if (ImGui::Button("Load Workspaces"))
    {
        m_workspaces = WorkspaceManager::Instance().GetWorkspaces();
    }
    for (const auto& w : m_workspaces.workspaces)
    {
        if (ImGui::TreeNode(w.name.c_str()))
        {
            for (const auto& r : w.requests)
            {
                if (ImGui::Selectable(r.name.c_str()))
                {
                    std::cout << r.name << std::endl;
                }
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

WorkspaceWindow& WorkspaceWindow::Instance()
{
    static WorkspaceWindow it;
    return it;
}

void WorkspaceWindow::SetWorkspaces(const Workspaces& workspaces)
{
    m_workspaces = workspaces;
}
