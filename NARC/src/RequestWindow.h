#pragma once

#include "imgui.h"
#include <thread>
#include <iostream>
#include <cpr/cpr.h>
#include "ResponseWindow.h"
#include "nlohmann/json.hpp"
#include <mutex>
#include "App.h"
#include <chrono>

using json = nlohmann::json;
class Request
{
public:
    void Draw() const;
    static Request& Get();
};
