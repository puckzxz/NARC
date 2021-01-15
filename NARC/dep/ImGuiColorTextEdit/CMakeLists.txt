cmake_minimum_required(VERSION 3.0)

add_library(ImGuiColorTextEdit TextEditor.cpp TextEditor.h)

target_link_libraries(ImGuiColorTextEdit PRIVATE ImGui)

target_include_directories(ImGuiColorTextEdit PUBLIC ./)