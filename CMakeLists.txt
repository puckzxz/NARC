cmake_minimum_required(VERSION 3.0)
set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(NARC)

file(GLOB NARC_SRC "NARC/src/*.cpp" "NARC/src/*.h")

add_executable(NARC ${NARC_SRC})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT NARC)

add_definitions(-DGLFW_INCLUDE_NONE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(NARC PRIVATE glfw)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(NARC PRIVATE cpr)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(NARC PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

add_subdirectory(NARC/dep/glad)
target_link_libraries(NARC PRIVATE glad)
add_subdirectory(NARC/dep/imgui)
target_link_libraries(NARC PRIVATE ImGui)
add_subdirectory(NARC/dep/ImGuiColorTextEdit)
target_link_libraries(NARC PRIVATE ImGuiColorTextEdit)
