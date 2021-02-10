@echo off

IF NOT EXIST vcpkg (
    git clone https://github.com/microsoft/vcpkg.git
)

PUSHD vcpkg

git checkout a84190e1deca1b6a466a82b439e5e4b9f8c41b0e

IF NOT EXIST vcpkg.exe (
    CALL bootstrap-vcpkg.bat
)

CALL vcpkg.exe install glfw3:x64-windows
CALL vcpkg.exe install cpr:x64-windows
CALL vcpkg.exe install nlohmann-json:x64-windows
CALL vcpkg.exe install ixwebsocket:x64-windows

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