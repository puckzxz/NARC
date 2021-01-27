#include "App.h"
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"

App::App()
{
    m_appWindow = nullptr;
    m_showDemoWindow = false;
    m_showWorkspacePanel = true;
    m_showRequestPanel = true;
    m_showResponsePanel = true;
}

App::~App()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_appWindow);
    glfwTerminate();
}

bool App::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_appWindow = glfwCreateWindow(App::Width, App::Height, "NARC", nullptr, nullptr);
    glfwMakeContextCurrent(m_appWindow);
    glfwSwapInterval(1);
    if (m_appWindow == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDockingWithShift = true;
    io.ConfigDockingTransparentPayload = true;
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    // style.WindowBorderSize = 0.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(m_appWindow, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    glViewport(0, 0, App::Width, App::Height);
    glfwSetFramebufferSizeCallback(m_appWindow, framebufferCallback);
    return true;
}

void App::Run()
{
    while (!glfwWindowShouldClose(m_appWindow))
    {
        glViewport(0, 0, App::Width, App::Height);
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    glfwSetWindowShouldClose(m_appWindow, true);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Toggle Workspace Panel"))
                {
                    m_showWorkspacePanel = !m_showWorkspacePanel;
                }
                if (ImGui::MenuItem("Toggle Request Panel"))
                {
                    m_showRequestPanel = !m_showRequestPanel;
                }
                if (ImGui::MenuItem("Toggle Response Panel"))
                {
                    m_showResponsePanel = !m_showResponsePanel;
                }
                if (ImGui::MenuItem("Toggle Demo Window"))
                {
                    m_showDemoWindow = !m_showDemoWindow;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (m_showWorkspacePanel)
        {
            WorkspaceWindow::Instance().Draw();
        }
        if (m_showRequestPanel)
        {
            RequestWindow::Instance().Draw();
        }
        if (m_showResponsePanel)
        {
            ResponseWindow::Instance().Draw();

        }

        if (m_showDemoWindow)
        {
            ImGui::ShowDemoWindow(&m_showDemoWindow);
        }
        ImGui::Render();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_appWindow);
    }
}

void App::framebufferCallback(GLFWwindow* window, const int width, const int height)
{
    App::Height = width;
    App::Width = height;
    glViewport(0, 0, width, height);
}
