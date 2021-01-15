#pragma once
#include "imgui.h"
#include <string>

#include "TextEditor.h"

// TODO: Convert to singleton pattern

class Response
{
public:
    Response();
    void Draw() const;
    static void SetJSON(const std::string data);

    inline static std::string JsonResponse;
};
