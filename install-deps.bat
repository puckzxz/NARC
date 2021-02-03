@echo off

git clone https://github.com/microsoft/vcpkg.git

pushd vcpkg

call bootstrap-vcpkg.bat

call vcpkg.exe install glfw3:x64-windows
call vcpkg.exe install cpr:x64-windows
call vcpkg.exe install nlohmann-json:x64-windows
call vcpkg.exe install ixwebsocket:x64-windows

move installed\x64-windows\share\ixwebsocket-config.cmake installed\x64-windows\share\ixwebsocket\ixwebsocket-config.cmake
move installed\x64-windows\share\ixwebsocket-config-release.cmake installed\x64-windows\share\ixwebsocket\ixwebsocket-config-release.cmake
move installed\x64-windows\share\ixwebsocket-config-debug.cmake installed\x64-windows\share\ixwebsocket\ixwebsocket-config-debug.cmake