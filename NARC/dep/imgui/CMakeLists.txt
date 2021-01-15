cmake_minimum_required(VERSION 3.0)

add_library(ImGui imconfig.h imgui.h imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_internal.h imgui_widgets.cpp imstb_rectpack.h imstb_textedit.h imstb_truetype.h imgui_demo.cpp backends/imgui_impl_opengl3.cpp backends/imgui_impl_opengl3.h backends/imgui_impl_glfw.cpp backends/imgui_impl_glfw.h)

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ImGui PRIVATE glfw)
target_link_libraries(ImGui PRIVATE glad)

target_include_directories(ImGui PUBLIC /)