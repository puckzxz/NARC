#pragma once

#include "ixwebsocket/IXSocketMbedTLS.h"
#include "ixwebsocket/IXNetSystem.h"
#include "ixwebsocket/IXWebSocket.h"

#include <iostream>
#include <string>
#include <memory>
#include "imgui.h"
#include "RequestWindow.h"
#include "ResponseWindow.h"
#include "WorkspaceWindow.h"
#include "WebsocketWindow.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "SettingsManager.h"

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
    static void framebufferCallback(GLFWwindow* window, const int width, const int height);
    Settings m_settings;
};
