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

class WorkspaceManager
{
public:
    static WorkspaceManager& Instance();
    void SaveWorkspace(const Workspace& ws);
    std::vector<Workspace>& GetWorkspaces();
private:
    WorkspaceManager(){};
    std::vector<Workspace> m_workspaces;
    void saveWorkspace(const std::string& fileName, const Workspace& ws);
    bool writeDefaultWorkspaceFile() const;
    inline std::string formatWorkspaceFileName(const std::string& name) const;
    const std::string m_folderName = "workspaces";
};
