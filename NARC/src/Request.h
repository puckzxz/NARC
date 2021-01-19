#pragma once

#include "imgui.h"
#include <thread>
#include <iostream>
#include <cpr/cpr.h>
#include "Response.h"
#include "nlohmann/json.hpp"
#include <mutex>
#include "App.h"

using json = nlohmann::json;

// TODO: Convert to singleton pattern

class Request
{
public:
    void Draw() const;
    static Request& Get();
};
