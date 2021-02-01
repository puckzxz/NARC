#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "imgui.h"
#include "RequestWindow.h"
#include "ResponseWindow.h"
#include "WorkspaceWindow.h"
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
    bool m_showWorkspacePanel;
    bool m_showRequestPanel;
    bool m_showResponsePanel;
    static void framebufferCallback(GLFWwindow* window, const int width, const int height);
    Settings m_settings;
};
