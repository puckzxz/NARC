#pragma once

#include "TextEditor.h"
#include "Window.h"
#include "cpr/cpr.h"

class ResponseWindow : public Window
{
public:
    void Draw() override;
    void SetResponse(const cpr::Response& resp);
    static ResponseWindow* Instance();

private:
    ResponseWindow();
    cpr::Response m_response;
    TextEditor m_editor;
};
