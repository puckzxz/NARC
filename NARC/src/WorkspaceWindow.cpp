#include "WorkspaceWindow.h"


#include "RequestWindow.h"
#include "WorkspaceManager.h"


void WorkspaceWindow::Draw()
{
    // TODO: Fix default item, refer to Request.cpp
    ImGui::Begin("Workspace");
    if (ImGui::BeginCombo("###Workspace", m_currentWorkspace.name.c_str()))
    {
        for (const auto& w : m_workspaces.workspaces)
        {
            if (ImGui::Selectable(w.name.c_str()))
            {
                m_currentWorkspace = w;
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::Button("Load Workspaces"))
    {
        m_workspaces = WorkspaceManager::Instance().GetWorkspaces();
    }
    for (const auto& r : m_currentWorkspace.requests)
    {
        if (ImGui::Selectable(std::string(r.type + " " + r.name).c_str()))
        {
            std::cout << r.name << " " << r.url << std::endl;
            RequestWindow::Instance().SetRequest(r);
        }
        // if (ImGui::TreeNode(w.name.c_str()))
        // {
        //
        //     ImGui::TreePop();
        // }
    }
    if (ImGui::BeginPopupContextWindow("###CTXMENU",
                                       ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
    {
        if (ImGui::Button("New Request"))
        {
            const Request r = {"test", "GET", "https://test.com"};
            m_currentWorkspace.requests.push_back(r);
            WorkspaceManager::Instance().SaveWorkspace(m_currentWorkspace);
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("New Folder"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
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
