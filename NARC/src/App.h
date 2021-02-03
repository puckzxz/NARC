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
private:
    GLFWwindow* m_appWindow;
    bool m_showDemoWindow;
    bool m_showWorkspaceWindow;
    bool m_showRequestWindow;
    bool m_showResponseWindow;
    bool m_showWebSocketWindow;
    static void framebufferCallback(GLFWwindow* window, const int width, const int height);
    Settings m_settings;
};
