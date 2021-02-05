#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

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

struct Workspaces
{
    std::vector<Workspace> workspaces;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Workspaces, workspaces);
};

class WorkspaceManager
{
public:
    static WorkspaceManager& Instance();
    bool WriteFile();
    void SaveWorkspace(const Workspace& ws);
    Workspaces GetWorkspaces();
private:
    WorkspaceManager(){};
    Workspaces m_workspaces;
    void saveWorkspaces();
    std::string m_fileName = "workspaces.narc";
};
