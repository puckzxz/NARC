@echo off

git clone https://github.com/microsoft/vcpkg.git

pushd vcpkg

call bootstrap-vcpkg.bat

call vcpkg.exe install glfw3:x64-windows
call vcpkg.exe install cpr:x64-windows
call vcpkg.exe install nlohmann-json:x64-windows