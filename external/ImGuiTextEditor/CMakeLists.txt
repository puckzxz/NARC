cmake_minimum_required(VERSION 3.0)

project(ImGuiTextEditor)

add_library(ImGuiTextEditor
    TextEditor.h
    TextEditor.cpp
)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(ImGuiTextEditor PRIVATE imgui::imgui)

target_include_directories(ImGuiTextEditor PUBLIC ./)