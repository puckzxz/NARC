#pragma once
#include "Console.h"
#ifdef _WIN32
#include "ixwebsocket/IXNetSystem.h"
#include "ixwebsocket/IXSocketMbedTLS.h"
#endif
#include "SettingsManager.h"
#include "GLFW/glfw3.h"
#include "Window.h"

class App
{
public:
    App();
    ~App();
    bool Init();
    void Run();
    inline static bool DemoWindowVisible;
    inline static const std::array<std::string, 7> RequestMethods = {
    "GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"
    };
private:
    inline static bool settingsWindowVisible;
    inline static bool m_consoleWindowVisible;
    GLFWwindow* m_appWindow;
    Settings m_settings;
    static void changeImGuiTheme(AppTheme theme);
    std::vector<Window*> m_windows;
};
