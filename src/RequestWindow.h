#pragma once

#include "App.h"
#include "TextEditor.h"
#include "WorkspaceWindow.h"
#include "nlohmann/json.hpp"
#include <cpr/cpr.h>

using json = nlohmann::json;

class RequestWindow : public Window
{
public:
    void Draw() override;
    void SetRequest(const Request& request);
    static RequestWindow* Instance();
    TextEditor* GetEditor() { return &m_editor; }
    void Reset();
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
    std::vector<std::pair<std::string, std::string>> m_headers;
    std::vector<std::pair<std::string, std::string>> m_queryParams;
    cpr::Header m_cprHeaders;

    int findRequestIndex(const std::string& requestMethodName);
};
