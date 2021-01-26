#pragma once
#include "imgui.h"
#include <string>
#include <iomanip>

#include "cpr/cpr.h"

#include "TextEditor.h"

class ResponseWindow
{
public:
    static ResponseWindow& Instance();
    void Draw() const;
    void SetResponse(const cpr::Response& resp);

private:
    ResponseWindow();
    cpr::Response m_response;
};
