cmake_minimum_required(VERSION 3.0)
set(CMAKE_TOOLCHAIN_FILE "./vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(NARC)

set(EXEC_NAME NARC)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXEC_NAME})

file(GLOB NARC_SRC "src/*.cpp" "src/*.h")

add_executable(${EXEC_NAME} ${NARC_SRC})

add_definitions(-DGLFW_INCLUDE_NONE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${EXEC_NAME} PRIVATE glfw)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(${EXEC_NAME} PRIVATE cpr)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${EXEC_NAME} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

find_package(ixwebsocket CONFIG REQUIRED)
target_link_libraries(${EXEC_NAME} PRIVATE ixwebsocket::ixwebsocket)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${EXEC_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)

add_subdirectory(external/glad)
target_link_libraries(${EXEC_NAME} PRIVATE glad)
add_subdirectory(external/imgui)
target_link_libraries(${EXEC_NAME} PRIVATE ImGui)
