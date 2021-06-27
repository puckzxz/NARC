#include "App.h"
#include <memory>
#include "imgui.h"
#include "imgui_internal.h"
#include "RequestWindow.h"
#include "ResponseWindow.h"
#include "WebsocketWindow.h"
#include "WorkspaceWindow.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "TextEditor.h"

App::App() : m_settings()
{
    m_appWindow = nullptr;
    DemoWindowVisible = false;
    settingsWindowVisible = false;
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
#ifdef NARC_DEBUG
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif
    Log::Init();
    Settings::Init();
    m_windows.push_back(WorkspaceWindow::Instance());
    m_windows.push_back(RequestWindow::Instance());
    m_windows.push_back(ResponseWindow::Instance());
    m_windows.push_back(WebsocketWindow::Instance());
    glfwSetErrorCallback([](const int id, const char* msg)
        {
            LOG_ERROR("id={0} msg={1}", id, msg);
        });
    if (!glfwInit())
    {
        NARC_ASSERT_NOT_REACHED("Failed to init GLFW");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    m_settings = Settings::Get();
    if (m_settings.maximized)
    {
        glfwWindowHint(GLFW_MAXIMIZED, 1);
    }
    m_appWindow = glfwCreateWindow(m_settings.windowWidth, m_settings.windowHeight, "NARC", nullptr, nullptr);
    if (m_appWindow == nullptr)
    {
        NARC_ASSERT_NOT_REACHED("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_appWindow);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        NARC_ASSERT_NOT_REACHED("Failed to initialize GLAD");
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
    io.ConfigDockingTransparentPayload = true;
    changeImGuiTheme(m_settings.theme);
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
            Settings::Save(app->m_settings);
        });
#ifdef _WIN32
    if (!ix::initNetSystem())
    {
        NARC_ASSERT_NOT_REACHED("Failed to init WebSockets");
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
                ImGui::MenuItem("Settings", nullptr, &settingsWindowVisible);
                if (ImGui::MenuItem("Exit"))
                {
                    glfwSetWindowShouldClose(m_appWindow, true);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                for (const auto& w : m_windows)
                {
                    ImGui::MenuItem(w->Name().c_str(), nullptr, &w->Visible);
                }
                ImGui::MenuItem("Demo Window", nullptr, &DemoWindowVisible);
                ImGui::MenuItem("Console", nullptr, &m_consoleWindowVisible);
                if (ImGui::MenuItem("Reset Layout"))
                {
                    ImGui::DockBuilderRemoveNode(dockSpaceId);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        ImGui::End();

        if (settingsWindowVisible)
        {
            static std::string saveButtonName = "Save";
            ImGui::Begin("Settings", &settingsWindowVisible);
            static std::array<std::string, 3> themes = { "Dark", "Light", "Classic" };
            if (ImGui::BeginCombo("Theme", themes.at(m_settings.theme).c_str()))
            {
                for (auto i = 0; i < themes.size(); i++)
                {
                    if (ImGui::Selectable(themes.at(i).c_str()))
                    {
                        m_settings.theme = static_cast<AppTheme>(i);
                        changeImGuiTheme(m_settings.theme);
                        saveButtonName = "Save*";
                    }
                }
                ImGui::EndCombo();
            }
            static auto pal = Settings::GetPalette();
            for (int i = 0; i < (int)TextEditor::PaletteIndex::Max; ++i)
            {
                auto color = ImGui::ColorConvertU32ToFloat4(pal[i]);
                color.w *= ImGui::GetStyle().Alpha;
                if (ImGui::ColorEdit4(std::string(Settings::PaletteIndexNames[i]).c_str(), &color.x))
                {
                    saveButtonName = "Save*";
                }
                pal[i] = ImGui::ColorConvertFloat4ToU32(color);
            }
            ResponseWindow::Instance()->GetEditor()->SetPalette(pal);
            RequestWindow::Instance()->GetEditor()->SetPalette(pal);
            if (ImGui::Button("Reset Palette"))
            {
                saveButtonName = "Save*";
                pal = Settings::GetDefaultPalette();
            }
            ImGui::SameLine();
            if (ImGui::Button(saveButtonName.c_str()))
            {
                m_settings.palette = pal;
                Settings::Save(m_settings);
                saveButtonName = "Save";
            }
            ImGui::End();
        }

        for (const auto& w : m_windows)
            if (w->Visible)
                w->Draw();

        if (DemoWindowVisible)
            ImGui::ShowDemoWindow(&DemoWindowVisible);

        if (m_consoleWindowVisible)
            Console::Draw("NARC: Console", &m_consoleWindowVisible);

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

void App::changeImGuiTheme(const AppTheme theme)
{
    switch (theme)
    {
    case Dark:
        ImGui::StyleColorsDark();
        break;
    case Light:
        ImGui::StyleColorsLight();
        break;
    case Classic:
        ImGui::StyleColorsClassic();
        break;
    default:
        ImGui::StyleColorsDark();
    }
}
