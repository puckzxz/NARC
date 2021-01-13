#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"

int WINDOW_HEIGHT = 1080;
int WINDOW_WIDTH = 1920;

void FramebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glViewport(0, 0, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "NARC", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDockingWithShift = true;
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    auto show_demo_window = true;
    while (!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();
        {
            ImGui::Begin("Window");
            if (ImGui::Button("Toggle Demo Window"))
            {
                show_demo_window = !show_demo_window;
            }
            ImGui::End();
        }
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);   
        }
        ImGui::Render();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
