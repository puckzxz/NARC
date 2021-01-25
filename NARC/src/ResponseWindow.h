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
    void SetJSON(const std::string& data);
    void SetResponseCode(const int32_t& code);
    void SetResponseDuration(const double& time);
    void SetResponseHeaders(const cpr::Header& headers);

private:
    ResponseWindow();
    std::string m_jsonResponse;
    int32_t m_responseCode;
    double m_responseDuration;
    cpr::Header m_responseHeaders;
};
