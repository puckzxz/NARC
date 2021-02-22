#pragma once

#include "TextEditor.h"
#include "cpr/cpr.h"

class ResponseWindow
{
public:
    static ResponseWindow& Instance();
    void Draw();
    void SetResponse(const cpr::Response& resp);

private:
    ResponseWindow();
    cpr::Response m_response;
    TextEditor m_editor;
};
