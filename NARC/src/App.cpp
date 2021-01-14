#include "App.h"
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"

App::App()
{
    _appWindow = nullptr;
    show_demo_window = true;
}

App::~App()
{
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(_appWindow);
    glfwTerminate();
}

bool App::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    _appWindow = glfwCreateWindow(App::Width, App::Height, "NARC", nullptr, nullptr);
    glfwMakeContextCurrent(_appWindow);
    glfwSwapInterval(1);
    if (_appWindow == nullptr)
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
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(_appWindow, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    glViewport(0, 0, App::Width, App::Height);
    glfwSetFramebufferSizeCallback(_appWindow, framebufferCallback);
    return true;
}

void App::Run()
{
    while (!glfwWindowShouldClose(_appWindow))
    {
        glViewport(0, 0, App::Width, App::Height);
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();
        // Workspace Window
        {
            static std::string comboName = std::string();
            ImGui::Begin("Workspace");
            if(ImGui::BeginCombo("", comboName.c_str()))
            {
                for (int i = 1; i < 6; i++)
                {
                    std::string itemName = "Workspace " + std::to_string(i);
                    if(ImGui::Selectable(itemName.c_str()))
                    {
                        comboName = itemName;
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Toggle Demo Window"))
            {
                show_demo_window = !show_demo_window;
            }
            ImGui::End();
        }
        // Request Window
        {
            ImGui::Begin("Request");
            ImGui::End();
        }

        // Response Window
        {
            ImGui::Begin("Response");
            ImGui::End();
        }
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);   
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
        glfwSwapBuffers(_appWindow);
    }
}

void App::framebufferCallback(GLFWwindow* window, const int width, const int height)
{
    App::Height = width;
    App::Width = height;
    glViewport(0, 0, width, height);
}
