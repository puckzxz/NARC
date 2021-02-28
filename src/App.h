#pragma once
#ifdef _WIN32
#include "ixwebsocket/IXNetSystem.h"
#include "ixwebsocket/IXSocketMbedTLS.h"
#endif
#include "SettingsManager.h"
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
    inline static bool settingsWindowVisible;
    GLFWwindow* m_appWindow;
    Settings m_settings;
    static void changeImGuiTheme(AppTheme theme);
};
