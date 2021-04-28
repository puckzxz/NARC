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
    static inline bool Loading;
    TextEditor* GetEditor() { return &m_editor; }
private:
    ResponseWindow();
    cpr::Response m_response;
    TextEditor m_editor;
};
