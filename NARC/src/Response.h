#pragma once
#include "imgui.h"
#include <string>

#include "TextEditor.h"

class Response
{
public:
    Response();
    static Response& Get();
    void Draw() const;
    void SetJSON(const std::string& data);
    void SetResponseCode(const int32_t& code);

private:
    std::string m_jsonResponse;
    int32_t m_responseCode;
};
