#include "WorkspaceWindow.h"


#include "RequestWindow.h"

#include "misc/cpp/imgui_stdlib.h"


void WorkspaceWindow::Draw()
{
    static bool showAddWorkspacePopup = false;
    static bool showAddRequestPopup = false;
    static bool showAddFolderPopup = false;
    // TODO: Fix default item, refer to Request.cpp
    ImGui::Begin("Workspace", &App::WorkspaceWindowVisible, ImGuiWindowFlags_AlwaysAutoResize);
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
                SaveWorkspace(m_currentWorkspace);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
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
            showAddWorkspacePopup = true;
        }
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
    if (showAddWorkspacePopup)
    {
        ImGui::OpenPopup("Add a new workspace");
        showAddWorkspacePopup = false;
    }
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
        static std::string requestName;
        ImGui::Text("Request Name:");
        ImGui::InputText("###RequestName", &requestName);
        static std::string requestURL;
        ImGui::Text("Request URL:");
        ImGui::InputText("###RequestURL", &requestURL);
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            const Request r = {requestName, m_requestName, requestURL};
            m_currentWorkspace.requests.push_back(r);
            SaveWorkspace(m_currentWorkspace);
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
            SaveWorkspace(m_currentWorkspace);
            ImGui::CloseCurrentPopup();
            folderName.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }
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

            AddWorkspace(workspaceName);
            ImGui::CloseCurrentPopup();
            workspaceName.clear();
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

void WorkspaceWindow::SaveWorkspace(const Workspace& ws)
{
    for (auto& w : m_workspaces)
    {
        if (w.name == ws.name)
        {
            w = ws;
            saveWorkspace(formatWorkspaceFileName(ws.name), w);
        }
    }
}

bool WorkspaceWindow::LoadWorkspaces()
{
    if (!std::filesystem::exists(formatWorkspaceFileName("Default")))
    {
        if (!writeDefaultWorkspaceFile())
        {
            NARC_ASSERT_NOT_REACHED("Failed to write workspace file")
        }
        return LoadWorkspaces();
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

void WorkspaceWindow::AddWorkspace(const std::string& name)
{
    if (std::filesystem::exists(formatWorkspaceFileName(name)))
    {
        return;
    }
    const std::vector<Request> reqs = {};
    const Workspace ws{name, reqs};
    try
    {
        std::ofstream os(formatWorkspaceFileName(ws.name));
        const json j = ws;
        os << j << std::endl;
        os.close();
    }
    catch (std::exception& e)
    {
        LOG_ERROR(e.what());
    }
    LoadWorkspaces();
}

void WorkspaceWindow::saveWorkspace(const std::string& fileName, const Workspace& ws)
{
    std::ofstream os(fileName);
    for (const auto& w : m_workspaces)
    {
        if (w.name == ws.name)
        {
            const json j = w;
            os << j << std::endl;
            os.close();
        }
    }
    LoadWorkspaces();
}

bool WorkspaceWindow::writeDefaultWorkspaceFile() const
{
    if (!std::filesystem::exists(m_folderName))
    {
        if (!std::filesystem::create_directory(m_folderName))
        {
            return false;
        }
    }
    const std::vector<Request> reqs = {};
    const Workspace ws{"Default", reqs};
    try
    {
        std::ofstream os(formatWorkspaceFileName(ws.name));
        const json j = ws;
        os << j << std::endl;
        os.close();
    }
    catch (std::exception& e)
    {
        LOG_ERROR(e.what());
        return false;
    }
    return true;
}

std::string WorkspaceWindow::formatWorkspaceFileName(const std::string& name) const
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
                RequestWindow::Instance().SetRequest(r);
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
