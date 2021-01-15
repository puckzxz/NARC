#pragma once

#include "imgui.h"
#include <cpr/cpr.h>
#include <iostream>
#include <string>
#include <thread>

// TODO: Convert to singleton pattern

class Request
{
public:
    void Draw() const;
};
