#pragma once

#include "imgui.h"
#include <thread>
#include <iostream>
#include <cpr/cpr.h>
#include "ResponseWindow.h"
#include "nlohmann/json.hpp"
#include <mutex>
#include "App.h"
#include <chrono>

#include "WorkspaceManager.h"

#include "TextEditor.h"

using json = nlohmann::json;
class RequestWindow
{
public:
    void Draw();
    static RequestWindow& Instance();
    void SetRequest(const Request& request);
private:
    RequestWindow();
    Request m_currentRequest;
    uint8_t m_requestTypeIndex = 0;
    std::array<std::string, 7> m_requestTypes = {
        "GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"
    };
    std::string m_requestType;
    std::string m_requestURL;
    std::string m_requestName = m_requestTypes[m_requestTypeIndex];
    TextEditor m_editor;
    std::string m_requestText;
};
