#include "ResponseWindow.h"

#include "RequestWindow.h"

TextEditor s_Editor;

ResponseWindow::ResponseWindow()
{
    m_responseCode = 0;
    m_jsonResponse = "";
    m_responseDuration = 0;
    s_Editor.SetReadOnly(true);
    s_Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::JSON());
    s_Editor.SetShowWhitespaces(false);
}

ResponseWindow& ResponseWindow::Get()
{
    static ResponseWindow it;
    return it;
}

void ResponseWindow::Draw() const
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
            if (ImGui::BeginTable("##Headers", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_RowBg))
            {
                for (const auto& [name, value] : m_responseHeaders)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text(name.c_str());
                    ImGui::TableNextColumn();
                    ImGui::TextWrapped(value.c_str());
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void ResponseWindow::SetJSON(const std::string& data)
{
    m_jsonResponse = data;
    s_Editor.SetText(m_jsonResponse);
}

void ResponseWindow::SetResponseCode(const int32_t& code)
{
    m_responseCode = code;
}

void ResponseWindow::SetResponseDuration(const double& time)
{
    m_responseDuration = time;
}

void ResponseWindow::SetResponseHeaders(const cpr::Header& headers)
{
    m_responseHeaders = headers;
}
