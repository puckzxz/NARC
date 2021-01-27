#include "ResponseWindow.h"

#include "RequestWindow.h"

TextEditor s_Editor;

ResponseWindow::ResponseWindow(): m_response()
{
    s_Editor.SetReadOnly(true);
    s_Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
    s_Editor.SetShowWhitespaces(false);
}

ResponseWindow& ResponseWindow::Instance()
{
    static ResponseWindow it;
    return it;
}

void ResponseWindow::Draw() const
{
    ImGui::Begin("Response");
    if (m_response.status_code >= 100 && m_response.status_code <= 199)
        ImGui::TextColored({ 150, 150, 150, 1 }, "%d", m_response.status_code);
    if (m_response.status_code >= 200 && m_response.status_code <= 299)
        ImGui::TextColored({ 0, 255, 0, 1 }, "%d", m_response.status_code);
    if (m_response.status_code >= 300 && m_response.status_code <= 399)
        ImGui::TextColored({ 255, 255, 0, 1 }, "%d", m_response.status_code);
    if (m_response.status_code >= 400 && m_response.status_code <= 499)
        ImGui::TextColored({ 255, 165, 0, 1 }, "%d", m_response.status_code);
    if (m_response.status_code >= 500 && m_response.status_code <= 599)
        ImGui::TextColored({ 255, 0, 0, 1 }, "%d", m_response.status_code);
    if (m_response.elapsed != 0)
    {
        ImGui::SameLine();
        ImGui::Text("%.2f ms", m_response.elapsed * 1000);
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
                for (const auto& [name, value] : m_response.header)
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
        if (ImGui::BeginTabItem("Cookies"))
        {
            if (ImGui::BeginTable("##Cookies", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_RowBg))
            {
                for (const auto& [name, value] : m_response.cookies)
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

void ResponseWindow::SetResponse(const cpr::Response& resp)
{
    m_response = resp;
    if (json::accept(resp.text))
    {
        s_Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::JSON());
        s_Editor.SetText(json::parse(resp.text).dump(4));
    }
    else
    {
        s_Editor.SetLanguageDefinition(TextEditor::LanguageDefinition::NONE());
        s_Editor.SetText(resp.text);
    }
}
