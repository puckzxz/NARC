#include "Response.h"

#include "Request.h"

TextEditor s_Editor;

Response::Response()
{
    m_responseCode = 0;
    m_jsonResponse = "";
    m_responseDuration = 0;
    s_Editor.SetReadOnly(true);
    s_Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::JSON());
    s_Editor.SetShowWhitespaces(false);
}

Response& Response::Get()
{
    static Response it;
    return it;
}

void Response::Draw() const
{
    ImGui::Begin("Response");
    if (m_responseCode >= 100 && m_responseCode <= 199)
        ImGui::TextColored({150, 150, 150, 1}, "%d", m_responseCode);
    if (m_responseCode >= 200 && m_responseCode <= 299)
        ImGui::TextColored({0, 255, 0, 1}, "%d", m_responseCode);
    if (m_responseCode >= 300 && m_responseCode <= 399)
        ImGui::TextColored({255, 255, 0, 1}, "%d", m_responseCode);
    if (m_responseCode >= 400 && m_responseCode <= 499)
        ImGui::TextColored({255, 165, 0, 1}, "%d", m_responseCode);
    if (m_responseCode >= 500 && m_responseCode <= 599)
        ImGui::TextColored({255, 0, 0, 1}, "%d", m_responseCode);
    if (m_responseDuration != 0)
    {
        ImGui::SameLine();
        ImGui::Text("%.3f", m_responseDuration);
    }
    s_Editor.Render("Text Editor");
    ImGui::End();
}

void Response::SetJSON(const std::string& data)
{
    m_jsonResponse = data;
    s_Editor.SetText(m_jsonResponse);
}

void Response::SetResponseCode(const int32_t& code)
{
    m_responseCode = code;
}

void Response::SetResponseDuration(const double& time)
{
    m_responseDuration = time;
}
