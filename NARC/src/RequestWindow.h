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

using json = nlohmann::json;
class RequestWindow
{
public:
    void Draw();
    static RequestWindow& Instance();
    void SetRequest(const Request& request);
private:
    RequestWindow(): m_requestUrlBuffer()
    {
    }

    Request m_currentRequest;
    uint8_t m_requestTypeIndex = 0;
    std::array<char, 4096> m_requestUrlBuffer;
};
