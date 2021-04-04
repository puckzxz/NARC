#include "ResponseWindow.h"

#include <string>
#include "App.h"
#include "imgui.h"
#include "RequestWindow.h"

ResponseWindow::ResponseWindow() : m_response()
{
    m_editor.SetReadOnly(true);
    m_name = "Response";
    m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::JSON());
    m_editor.SetShowWhitespaces(false);
    if (SettingsManager::GetSettings().theme == AppTheme::Light)
        m_editor.SetPalette(TextEditor::GetLightPalette());
}

void ResponseWindow::Draw()
{
    if (SettingsManager::GetSettings().theme == AppTheme::Light)
        m_editor.SetPalette(TextEditor::GetLightPalette());
    else
        m_editor.SetPalette(TextEditor::GetDarkPalette());
    ImGui::Begin("Response", &Visible, ImGuiWindowFlags_AlwaysAutoResize);
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
            this->m_editor.Render("Text Editor");
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
        m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::JSON());
        m_editor.SetText(json::parse(resp.text).dump(4));
    }
    else
    {
        m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::None());
        m_editor.SetText(resp.text);
    }
}

ResponseWindow* ResponseWindow::Instance()
{
    static ResponseWindow it;
    return &it;    
}
