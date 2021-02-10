if [ ! -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg.git
fi

cd vcpkg

git checkout a84190e1deca1b6a466a82b439e5e4b9f8c41b0e

if [ ! -f "vcpkg" ]; then
    sh ./bootstrap-vcpkg.sh
fi

./vcpkg install glfw3:x64-linux
./vcpkg install cpr:x64-linux
./vcpkg install nlohmann-json:x64-linux
./vcpkg install ixwebsocket:x64-linux

cd installed/x64-linux/share

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