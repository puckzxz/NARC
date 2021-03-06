﻿#include "WorkspaceWindow.h"

#include "RequestWindow.h"

#include "imgui_stdlib.h"

void WorkspaceWindow::Draw()
{
    // TODO: Fix default item, refer to Request.cpp
    ImGui::Begin("Workspace", &Visible, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::BeginCombo("###Workspace", m_currentWorkspace.name.c_str()))
    {
        for (const auto& w : m_workspaces)
        {
            if (ImGui::Selectable(w.name.c_str()))
            {
                m_currentWorkspace = w;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    if (ImGui::BeginTable("###RequestTable", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("Method", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
        for (const auto& r : m_currentWorkspace.requests)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(r.type.c_str());
            ImGui::TableNextColumn();
            if (ImGui::Selectable(r.name.c_str()))
            {
                RequestWindow::Instance()->SetRequest(r);
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
                    saveWorkspace(m_currentWorkspace);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::EndTable();
    }
    for (const auto& f : m_currentWorkspace.folders)
    {
        listAllItemsInFolder(f);
    }
    if (ImGui::BeginPopupContextWindow("Workspace",
        ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
    {
        if (ImGui::Button("New Workspace"))
        {
            // TODO: Create new workspace
            ImGui::CloseCurrentPopup();
            m_showAddWorkspacePopup = true;
        }
        if (ImGui::Button("New Request"))
        {
            ImGui::CloseCurrentPopup();
            m_showAddRequestPopup = true;
        }
        if (ImGui::Button("New Folder"))
        {
            ImGui::CloseCurrentPopup();
            m_showAddFolderPopup = true;
        }
        ImGui::EndPopup();
    }
    // TODO: Cleanup
    if (m_showAddWorkspacePopup)
    {
        ImGui::OpenPopup("Add a new workspace");
        m_showAddWorkspacePopup = false;
    }
    if (m_showAddRequestPopup)
    {
        ImGui::OpenPopup("Add a new request");
        m_showAddRequestPopup = false;
    }
    if (m_showAddFolderPopup)
    {
        ImGui::OpenPopup("Add a new folder");
        m_showAddFolderPopup = false;
    }
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Add a new request", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static uint8_t m_requestTypeIndex = 0;
        static std::string m_requestName = App::RequestMethods[m_requestTypeIndex];
        ImGui::Text("Request Method:");
        if (ImGui::BeginCombo("###RequestType", App::RequestMethods.at(m_requestTypeIndex).c_str()))
        {
            for (auto i = 0; i < App::RequestMethods.size(); i++)
            {
                if (ImGui::Selectable(App::RequestMethods.at(i).c_str()))
                {
                    m_requestTypeIndex = i;
                    m_requestName = App::RequestMethods[i];
                }
            }
            ImGui::EndCombo();
        }
        static std::string requestName;
        ImGui::Text("Request Name:");
        ImGui::InputText("###RequestName", &requestName);
        static std::string requestURL;
        ImGui::Text("Request URL:");
        ImGui::InputText("###RequestURL", &requestURL);
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            const Request r = { requestName, m_requestName, requestURL };
            m_currentWorkspace.requests.push_back(r);
            saveWorkspace(m_currentWorkspace);
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
        static std::string folderName;
        ImGui::InputText("Folder Name", &folderName);
        if (ImGui::Button("OK"))
        {
            Folder f;
            f.name = folderName;
            m_currentWorkspace.folders.push_back(f);
            saveWorkspace(m_currentWorkspace);
            ImGui::CloseCurrentPopup();
            folderName.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopupModal("Add a new workspace", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        struct TextFilters
        {
            static int FilterInvalidWorkspaceName(ImGuiInputTextCallbackData* data)
            {
                switch (data->EventChar)
                {
                    // Unix
                case '/':
                    // Windows
                case '<':
                case '>':
                case ':':
                case '\"':
                case '\\':
                case '|':
                case '?':
                case '*':
                case ' ':
                    return 1;
                default:
                    return 0;
                }
            }
        };
        static std::string workspaceName;
        ImGui::InputText("Workspace Name", &workspaceName, ImGuiInputTextFlags_CallbackCharFilter,
            TextFilters::FilterInvalidWorkspaceName);
        if (ImGui::Button("Add"))
        {
#ifdef _WIN32
            // Windows has reserved filenames
            if (std::find(m_invalidFolderNames.begin(), m_invalidFolderNames.end(), workspaceName) !=
                m_invalidFolderNames.end())
            {
                workspaceName.clear();
                return;
            }
            // Windows filenames cannot end in a "."
            if (workspaceName.back() == '.')
            {
                workspaceName.pop_back();
                return;
            }
#endif
            addWorkspace(workspaceName);
            ImGui::CloseCurrentPopup();
            workspaceName.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}

WorkspaceWindow* WorkspaceWindow::Instance()
{
    static WorkspaceWindow it;
    return &it;    
}

void WorkspaceWindow::saveWorkspace(const Workspace& ws)
{
    for (auto& w : m_workspaces)
    {
        if (w.name == ws.name)
        {
            w = ws;
            std::ofstream os(formatWorkspaceFileName(ws.name));
            const json j = w;
            os << j << std::endl;
            os.close();
        }
    }
}

bool WorkspaceWindow::loadWorkspaces()
{
    if (!std::filesystem::exists(formatWorkspaceFileName("Default")))
    {
        if (!writeDefaultWorkspaceFile())
        {
            NARC_ASSERT_NOT_REACHED("Failed to write workspace file")
        }
    }
    m_workspaces.clear();
    for (const auto& entry : std::filesystem::directory_iterator(m_folderName))
    {
        json j;
        std::ifstream is(entry.path());
        is >> j;
        is.close();
        m_workspaces.push_back(j.get<Workspace>());
    }
    return true;
}

void WorkspaceWindow::addWorkspace(const std::string& name)
{
    if (std::filesystem::exists(formatWorkspaceFileName(name)))
    {
        return;
    }
    const std::vector<Request> reqs = {};
    const Workspace ws{ name, reqs };
    try
    {
        std::ofstream os(formatWorkspaceFileName(ws.name));
        const json j = ws;
        os << j << std::endl;
        os.close();
    }
    catch (std::exception& e)
    {
        Console::Log(e.what());
    }
    loadWorkspaces();
}

bool WorkspaceWindow::writeDefaultWorkspaceFile()
{
    if (!std::filesystem::exists(m_folderName))
    {
        if (!std::filesystem::create_directory(m_folderName))
        {
            return false;
        }
    }
    const std::vector<Request> reqs = {};
    const Workspace ws{ "Default", reqs };
    m_workspaces.emplace_back(ws);
    try
    {
        saveWorkspace(ws);
    }
    catch (std::exception& e)
    {
        Console::Log(e.what());
        return false;
    }
    return true;
}

std::string WorkspaceWindow::formatWorkspaceFileName(const std::string& name)
{
    return m_folderName + "/" + name + ".narc";
}

void WorkspaceWindow::listAllItemsInFolder(const Folder& folder)
{
    for (const auto& f : folder.folders)
    {
        listAllItemsInFolder(f);
    }
    if (ImGui::CollapsingHeader(folder.name.c_str()))
    {
        for (const auto& r : folder.requests)
        {
            if (ImGui::Selectable(std::string(r.type + " " + r.name).c_str()))
            {
                RequestWindow::Instance()->SetRequest(r);
            }
        }
    }
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::Button("Add Request"))
        {
        }
        if (ImGui::Button("Add Folder"))
        {
        }
        if (ImGui::Button("Delete"))
        {
        }
        ImGui::EndPopup();
    }
}
