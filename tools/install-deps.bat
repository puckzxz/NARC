@echo off

IF NOT EXIST vcpkg (
    git clone https://github.com/microsoft/vcpkg.git
)

PUSHD vcpkg

git checkout 105456798402aa5f494ffeb3b19dd0d870656d39

IF NOT EXIST vcpkg.exe (
    CALL bootstrap-vcpkg.bat
)

CALL vcpkg.exe install glfw3:x64-windows
CALL vcpkg.exe install cpr:x64-windows
CALL vcpkg.exe install nlohmann-json:x64-windows
CALL vcpkg.exe install ixwebsocket:x64-windows
CALL vcpkg.exe install spdlog:x64-windows
CALL vcpkg.exe install fmt:x64-windows
CALL vcpkg.exe install imgui[docking-experimental,opengl3-glad-binding,glfw-binding]:x64-windows

PUSHD installed\x64-windows\share

IF EXIST ixwebsocket-config.cmake (
    MOVE ixwebsocket-config.cmake ixwebsocket\ixwebsocket-config.cmake
)
IF EXIST ixwebsocket-config-release.cmake (
    MOVE ixwebsocket-config-release.cmake ixwebsocket\ixwebsocket-config-release.cmake
)
IF EXIST ixwebsocket-config-debug.cmake (
    MOVE ixwebsocket-config-debug.cmake ixwebsocket\ixwebsocket-config-debug.cmake
)