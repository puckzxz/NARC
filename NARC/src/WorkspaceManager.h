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
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Request, name, type);
};

struct Workspace
{
    std::string name;
    std::vector<Request> requests;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Workspace, name, requests);
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
    static bool WriteFile();
    static bool DeleteFile();
    Workspaces GetWorkspaces() const;
private:
    WorkspaceManager(){};
};
