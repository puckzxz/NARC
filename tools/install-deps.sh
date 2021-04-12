#!/bin/bash

if [ ! -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg.git
fi

cd vcpkg || exit

git checkout 105456798402aa5f494ffeb3b19dd0d870656d39

if [ ! -f "vcpkg" ]; then
    sh ./bootstrap-vcpkg.sh
fi

./vcpkg install glfw3:x64-linux
./vcpkg install cpr:x64-linux
./vcpkg install nlohmann-json:x64-linux
./vcpkg install ixwebsocket:x64-linux
./vcpkg install spdlog:x64-linux
./vcpkg install fmt:x64-linux
./vcpkg install imgui[docking-experimental,opengl3-glad-binding,glfw-binding]:x64-linux

cd installed/x64-linux/share || exit

if [ -f "ixwebsocket-config.cmake" ]; then
    mv ./ixwebsocket-config.cmake ./ixwebsocket/ixwebsocket-config.cmake
fi

if [ -f "ixwebsocket-config-release.cmake" ]; then
    mv ./ixwebsocket-config-release.cmake ./ixwebsocket/ixwebsocket-config-release.cmake
fi

if [ -f "ixwebsocket-config-debug.cmake" ]; then 
    mv ./ixwebsocket-config-debug.cmake ./ixwebsocket/ixwebsocket-config-debug.cmake
fi

# sudo apt install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev