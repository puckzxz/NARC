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
        ImGui::TextColored({ 150, 150, 150, 1 }, "%d", m_responseCode);
    if (m_responseCode >= 200 && m_responseCode <= 299)
        ImGui::TextColored({ 0, 255, 0, 1 }, "%d", m_responseCode);
    if (m_responseCode >= 300 && m_responseCode <= 399)
        ImGui::TextColored({ 255, 255, 0, 1 }, "%d", m_responseCode);
    if (m_responseCode >= 400 && m_responseCode <= 499)
        ImGui::TextColored({ 255, 165, 0, 1 }, "%d", m_responseCode);
    if (m_responseCode >= 500 && m_responseCode <= 599)
        ImGui::TextColored({ 255, 0, 0, 1 }, "%d", m_responseCode);
    if (m_responseDuration != 0)
    {
        ImGui::SameLine();
        ImGui::Text("%.3f", m_responseDuration);
    }
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("JSON"))
        {
            s_Editor.Render("Text Editor");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Headers"))
        {
            ImVec2 outer_size = ImVec2(-FLT_MIN, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
            if (ImGui::BeginTable("##Headers", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_RowBg))
            {
                for (const auto& header : m_responseHeaders)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text(header.first.c_str());
                    ImGui::TableNextColumn();
                    ImGui::TextWrapped(header.second.c_str());
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
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

void Response::SetResponseHeaders(const cpr::Header& headers)
{
    m_responseHeaders = headers;
}
