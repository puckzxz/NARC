#pragma once

#include "imgui.h"
#include <cpr/cpr.h>
#include <array>
#include <iostream>
#include <string>
#include <thread>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// TODO: Convert to singleton pattern

class Request
{
public:
    void Draw() const;

private:
    inline static std::array<char, 2048> _requestUrlBuffer = {};
};
