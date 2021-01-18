#pragma once

#include "imgui.h"
#include <thread>
#include <iostream>
#include <cpr/cpr.h>
#include "Response.h"
#include "nlohmann/json.hpp"
#include <mutex>

using json = nlohmann::json;

static std::mutex RequestMutex;

// TODO: Convert to singleton pattern

class Request
{
public:
    void Draw() const;

private:
};
