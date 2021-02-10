#include "WorkspaceWindow.h"


#include "RequestWindow.h"
#include "WorkspaceManager.h"

#include "misc/cpp/imgui_stdlib.h"


void WorkspaceWindow::Draw()
{
    // TODO: Fix default item, refer to Request.cpp
    ImGui::Begin("Workspace", &App::WorkspaceWindowVisible, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PushItemWidth(-FLT_MIN);
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
    ImGui::PopItemWidth();
    if (ImGui::BeginPopupContextItem("###WorkspaceComboCTX"))
    {
        if (ImGui::Button("New Workspace"))
        {
            // TODO: Create new workspace
        }
    }
    for (const auto& r : m_currentWorkspace.requests)
    {
        if (ImGui::Selectable(std::string(r.type + " " + r.name).c_str()))
        {
            RequestWindow::Instance().SetRequest(r);
        }
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::Button("Delete"))
            {
                m_currentWorkspace.requests.erase(std::remove_if(m_currentWorkspace.requests.begin(),
                                                                 m_currentWorkspace.requests.end(),
                                                                 [&](Request const& x)
                                                                 {
                                                                     return x.name == r.name;
                                                                 }), m_currentWorkspace.requests.end());
                WorkspaceManager::Instance().SaveWorkspace(m_currentWorkspace);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    static bool showAddRequestPopup = false;
    static bool showAddFolderPopup = false;
    if (ImGui::BeginPopupContextWindow("Workspace",
                                       ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
    {
        if (ImGui::Button("New Request"))
        {
            ImGui::CloseCurrentPopup();
            showAddRequestPopup = true;
        }
        if (ImGui::Button("New Folder"))
        {
            ImGui::CloseCurrentPopup();
            showAddFolderPopup = true;
        }
        ImGui::EndPopup();
    }
    // TODO: Cleanup
    if (showAddRequestPopup)
    {
        ImGui::OpenPopup("Add a new request");
        showAddRequestPopup = false;
    }
    if (showAddFolderPopup)
    {
        ImGui::OpenPopup("Add a new folder");
        showAddFolderPopup = false;
    }
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Add a new request", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static uint8_t m_requestTypeIndex = 0;
        static std::array<std::string, 7> m_requestTypes = {
            "GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"
        };
        static std::string m_requestName = m_requestTypes[m_requestTypeIndex];
        ImGui::Text("Request Method:");
        if (ImGui::BeginCombo("###RequestType", m_requestTypes.at(m_requestTypeIndex).c_str()))
        {
            for (auto i = 0; i < m_requestTypes.size(); i++)
            {
                if (ImGui::Selectable(m_requestTypes.at(i).c_str()))
                {
                    m_requestTypeIndex = i;
                    m_requestName = m_requestTypes[i];
                }
            }
            ImGui::EndCombo();
        }
        static std::string requestName = "";
        ImGui::Text("Request Name:");
        ImGui::InputText("###RequestName", &requestName);
        static std::string requestURL = "";
        ImGui::Text("Request URL:");
        ImGui::InputText("###RequestURL", &requestURL);
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            const Request r = {requestName, m_requestName, requestURL};
            m_currentWorkspace.requests.push_back(r);
            WorkspaceManager::Instance().SaveWorkspace(m_currentWorkspace);
            ImGui::CloseCurrentPopup();
            requestName.clear();
            requestURL.clear();
            m_requestTypeIndex = 0;
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopupModal("Add a new folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static std::string folderName = "";
        ImGui::InputText("Folder Name", &folderName);
        if (ImGui::Button("OK"))
        {
            Folder f;
            f.name = folderName;
            m_currentWorkspace.folders.push_back(f);
            WorkspaceManager::Instance().SaveWorkspace(m_currentWorkspace);
            ImGui::CloseCurrentPopup();
            folderName.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
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
