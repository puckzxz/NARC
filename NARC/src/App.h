#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "imgui.h"
#include "Request.h"
#include "Response.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Workspace.h"

class App
{
public:
    App();
    ~App();
    bool Init();
    void Run();
    inline static int Width = 1280;
    inline static int Height = 720;
private:
    GLFWwindow* _appWindow;
    bool _show_demo_window;
    Workspace _workspace;
    Request _request;
    Response _response;
private:
    static void framebufferCallback(GLFWwindow* window, const int width, const int height);
};
