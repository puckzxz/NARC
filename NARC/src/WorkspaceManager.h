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
    // static void to_json(json& j, const Request& r) {
    //     j = json{{"name", r.name}};
    // }
    //
    // static void from_json(const json& j, Request& r) {
    //     j.at("name").get_to(r.name);
    // }
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Request, name, type);
};

struct Workspace
{
    std::string name;
    std::vector<Request> requests;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Workspace, name, requests);
    // static void to_json(json& j, const Workspace& w) {
    //     j = json{{"name", w.name}};
    //     for (auto& e : w.requests)
    //     {
    //         j["requests"].push_back({{"name", e.name}});
    //     }
    // }
    //
    // static void from_json(const json& j, Workspace& w) {
    //     j.at("name").get_to(w.name);
    //     j.at("requests").get_to<std::vector<Request>>(w.requests);
    // }
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
