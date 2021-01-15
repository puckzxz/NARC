﻿#pragma once

#include "imgui.h"
#include <cpr/cpr.h>
#include <array>
#include <iostream>
#include <string>
#include <thread>
#include "nlohmann/json.hpp"

#include "Response.h"

using json = nlohmann::json;

// TODO: Convert to singleton pattern

class Request
{
public:
    void Draw() const;

private:
};
