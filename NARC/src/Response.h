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
    static void SetJSON(const std::string& data);
    static void SetResponseCode(const int32_t& code);

    inline static std::string JsonResponse;
    inline static int32_t ResponseCode;
};
