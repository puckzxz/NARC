#include "App.h"

#include "imgui_internal.h"
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
    auto& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
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

        const auto windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

        auto* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("RootWindow", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        const auto dockSpaceId = ImGui::GetID("DockSpace");
        if (!ImGui::DockBuilderGetNode(dockSpaceId))
        {
            ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockSpaceId, viewport->Size);

            auto dockMainId = dockSpaceId;
            const auto dockMiddleId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.5f, nullptr, &dockMainId);
            const auto dockRightId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.7f, nullptr, &dockMainId);

            ImGui::DockBuilderDockWindow("Workspace", dockMainId);
            ImGui::DockBuilderDockWindow("Response", dockMiddleId);
            ImGui::DockBuilderDockWindow("Request", dockRightId);

            ImGui::DockBuilderFinish(dockMainId);
        }

        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
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
                if (ImGui::MenuItem("Reset Layout"))
                {
                    ImGui::DockBuilderRemoveNode(dockSpaceId);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        ImGui::End();

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
