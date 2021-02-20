#include "App.h"


#include "Assert.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"

App::App(): m_settings()
{
    m_appWindow = nullptr;
    DemoWindowVisible = false;
    WorkspaceWindowVisible = true;
    RequestWindowVisible = true;
    ResponseWindowVisible = true;
    WebSocketWindowVisible = false;
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
    Log::Init();
    glfwSetErrorCallback([](const int id, const char* msg)
    {
        LOG_ERROR("id={0} msg={1}", id, msg);
    });
    if (!glfwInit())
    {
        LOG_ERROR("Failed to init GLFW");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    m_settings = SettingsManager::Instance().GetSettings();
    if (m_settings.maximized)
    {
        glfwWindowHint(GLFW_MAXIMIZED, 1);
    }
    m_appWindow = glfwCreateWindow(m_settings.windowWidth, m_settings.windowHeight, "NARC", nullptr, nullptr);
    if (m_appWindow == nullptr)
    {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_appWindow);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        LOG_ERROR("Failed to initialize GLAD");
        return false;
    }
    glfwSetWindowSize(m_appWindow, m_settings.windowWidth, m_settings.windowHeight);
    glfwSwapInterval(1);
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
    style.WindowMinSize = ImVec2(100.0f, 100.0f);
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(m_appWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    glViewport(0, 0, m_settings.windowWidth, m_settings.windowHeight);
    glfwSetWindowUserPointer(m_appWindow, this);
    glfwSetFramebufferSizeCallback(m_appWindow, [](GLFWwindow* window, const int width, const int height)
    {
        glViewport(0, 0, width, height);
    });
    glfwSetWindowSizeCallback(m_appWindow, [](GLFWwindow* window, const int width, const int height) mutable
    {
        auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        app->m_settings.windowHeight = height;
        app->m_settings.windowWidth = width;
        const bool maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
        app->m_settings.maximized = maximized;
        SettingsManager::Instance().SaveSettings(app->m_settings);
    });
#ifdef _WIN32
    if (!ix::initNetSystem())
    {
        LOG_ERROR("Failed to init WebSockets");
        return false;
    }
#endif
    return true;
}

void App::Run()
{
    while (!glfwWindowShouldClose(m_appWindow))
    {
        glViewport(0, 0, m_settings.windowWidth, m_settings.windowHeight);
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
            const auto dockMiddleId = ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.5f, nullptr,
                                                                  &dockMainId);
            const auto dockRightId =
                ImGui::DockBuilderSplitNode(dockMainId, ImGuiDir_Right, 0.7f, nullptr, &dockMainId);

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
                ImGui::MenuItem("Workspace Window", nullptr, &WorkspaceWindowVisible);
                ImGui::MenuItem("Request Window", nullptr, &RequestWindowVisible);
                ImGui::MenuItem("Response Window", nullptr, &ResponseWindowVisible);
                ImGui::MenuItem("WebSocket Window", nullptr, &WebSocketWindowVisible);
                ImGui::MenuItem("Demo Window", nullptr, &DemoWindowVisible);
                if (ImGui::MenuItem("Reset Layout"))
                {
                    ImGui::DockBuilderRemoveNode(dockSpaceId);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        ImGui::End();

        if (WorkspaceWindowVisible)
            WorkspaceWindow::Instance().Draw();
        
        if (RequestWindowVisible)
            RequestWindow::Instance().Draw();
        
        if (ResponseWindowVisible)
            ResponseWindow::Instance().Draw();
        
        if (DemoWindowVisible)
            ImGui::ShowDemoWindow(&DemoWindowVisible);
        
        if (WebSocketWindowVisible)
            WebsocketWindow::Instance().Draw();

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
