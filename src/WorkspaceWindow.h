﻿#pragma once
#include <string>
#include "imgui.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "Window.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "App.h"
#include "Assert.h"

struct Request
{
    std::string name;
    std::string type;
    std::string url;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Request, name, type, url);
};

struct Folder
{
    std::string name;
    std::vector<Folder> folders;
    std::vector<Request> requests;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Folder, name, folders, requests);
};

struct Workspace
{
    std::string name;
    std::vector<Request> requests;
    std::vector<Folder> folders;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Workspace, name, requests, folders);
};

class WorkspaceWindow : public Window
{
public:
    void Draw() override;
    static WorkspaceWindow* Instance();
private:
    WorkspaceWindow();
    void saveWorkspace(const Workspace& ws);
    bool loadWorkspaces();
    void addWorkspace(const std::string& name);
    bool writeDefaultWorkspaceFile();
    inline std::string formatWorkspaceFileName(const std::string& name);
    const std::string m_folderName = "workspaces";
    void listAllItemsInFolder(const Folder& folder);
    bool m_showAddWorkspacePopup;
    bool m_showAddRequestPopup;
    bool m_showAddFolderPopup;
    std::vector<Workspace> m_workspaces;
    Workspace m_currentWorkspace;
    static inline std::vector m_invalidFolderNames = {
        "CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "LPT1",
        "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
    };
    // bool m_visible;
};

inline WorkspaceWindow::WorkspaceWindow()
{
    m_showAddWorkspacePopup = false;
    m_showAddRequestPopup = false;
    m_showAddFolderPopup = false;
    m_name = "Workspace";
    if (!loadWorkspaces())
    {
        NARC_ASSERT_NOT_REACHED("Failed to load workspaces");
    }
    if (m_workspaces.size() > 0)
    {
        m_currentWorkspace = m_workspaces.front();
    }
}
