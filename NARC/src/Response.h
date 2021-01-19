#pragma once
#include "imgui.h"
#include <string>

#include "TextEditor.h"

class Response
{
public:
    static Response& Get();
    void Draw() const;
    void SetJSON(const std::string& data);
    void SetResponseCode(const int32_t& code);

private:
    Response();
    std::string m_jsonResponse;
    int32_t m_responseCode;
};
