#pragma once
#include "imgui.h"
#include <string>

// TODO: Convert to singleton pattern

class Response
{
public:
    void Draw() const;

    inline static std::string JsonResponse;
};
