#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "imgui.h"
#ifdef _WIN32
#include "ixwebsocket/IXSocketMbedTLS.h"
#include "ixwebsocket/IXNetSystem.h"
#endif
#include "ixwebsocket/IXWebSocket.h"
#include "spdlog/spdlog.h"
#include "RequestWindow.h"
#include "ResponseWindow.h"
#include "WorkspaceWindow.h"
#include "WebsocketWindow.h"
#include "SettingsManager.h"
#include "Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


class App
{
public:
    App();
    ~App();
    bool Init();
    void Run();
    inline static bool DemoWindowVisible;
    inline static bool WorkspaceWindowVisible;
    inline static bool RequestWindowVisible;
    inline static bool ResponseWindowVisible;
    inline static bool WebSocketWindowVisible;
private:
    GLFWwindow* m_appWindow;
    Settings m_settings;
};
