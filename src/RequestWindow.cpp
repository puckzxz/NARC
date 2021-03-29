#include "RequestWindow.h"

#include <thread>
#include <cpr/cpr.h>
#include "Assert.h"
#include "imgui.h"
#include "ResponseWindow.h"
#include "imgui_stdlib.h"

RequestWindow::RequestWindow()
{
    m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::JSON());
    m_editor.SetShowWhitespaces(false);
    m_queryParams.push_back(std::pair<std::string, std::string>("", ""));
    if (SettingsManager::Instance().GetSettings().theme == AppTheme::Light)
        m_editor.SetPalette(TextEditor::GetLightPalette());
}

void RequestWindow::Draw()
{
    if (SettingsManager::Instance().GetSettings().theme == AppTheme::Light)
        m_editor.SetPalette(TextEditor::GetLightPalette());
    else
        m_editor.SetPalette(TextEditor::GetDarkPalette());
    ImGui::Begin("Request", &App::RequestWindowVisible, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::BeginCombo("###RequestType", m_requestTypes.at(m_requestTypeIndex).c_str()))
    {
        for (auto i = 0; i < m_requestTypes.size(); i++)
        {
            if (ImGui::Selectable(m_requestTypes.at(i).c_str()))
            {
                m_requestTypeIndex = i;
                m_requestName = m_requestTypes[i];
            }
        }
        ImGui::EndCombo();
    }
    ImGui::InputText("###RequestURL", &m_requestURL);
    ImGui::PopItemWidth();
    if (ImGui::Button("Send"))
    {
        std::thread{
            [&]() -> void
            {
                cpr::Response resp;
                if (m_requestName == "GET")
                    resp = cpr::Get(cpr::Url{m_requestURL}, cpr::Body{m_editor.GetText()});
                else if (m_requestName == "POST")
                    // TODO: Change this to inputs
                    resp = cpr::Post(cpr::Url{m_requestURL}, cpr::Body{m_editor.GetText()}, cpr::Header{
                                         {"Content-Type", "application/json"}
                                     });
                else if (m_requestName == "PUT")
                    resp = cpr::Put(cpr::Url{m_requestURL}, cpr::Body{m_editor.GetText()});
                else if (m_requestName == "PATCH")
                    resp = cpr::Patch(cpr::Url{m_requestURL}, cpr::Body{m_editor.GetText()});
                else if (m_requestName == "DELETE")
                    resp = cpr::Delete(cpr::Url{m_requestURL}, cpr::Body{m_editor.GetText()});
                else if (m_requestName == "OPTIONS")
                    resp = cpr::Options(cpr::Url{m_requestURL}, cpr::Body{m_editor.GetText()});
                else if (m_requestName == "HEAD")
                    resp = cpr::Head(cpr::Url{m_requestURL}), cpr::Body{m_editor.GetText()};
                else
                    NARC_ASSERT_NOT_REACHED("Invalid REST Method");
                ResponseWindow::Instance().SetResponse(resp);
            }
        }.detach();
    }
    ImGui::Separator();
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("RequestTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("JSON"))
        {
            if (ImGui::Button("Format"))
            {
                const auto text = m_editor.GetText();
                if (json::accept(text))
                {
                    m_editor.SetText(json::parse(text).dump(4));
                }
            }
            m_editor.Render("Request Editor");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Query"))
        {
            static std::string urlPreview;
            ImGui::PushItemWidth(-FLT_MIN);
            ImGui::InputText("###URLPreview", &urlPreview, ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
            for (size_t i = 0; i < m_queryParams.size(); i++)
            {
                std::pair<std::string, std::string> q = m_queryParams.at(i);
                for (size_t x = 0; x < 2; x++)
                {
                    if (x % 2 == 0)
                    {
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() / 2);
                        ImGui::InputText(std::string("###inpq" + std::to_string(i)).c_str(), &q.first);
                        ImGui::PopItemWidth();
                        ImGui::SameLine();
                    }
                    else
                    {
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 23);
                        ImGui::InputText(std::string("###inps" + std::to_string(i)).c_str(), &q.second);
                        ImGui::PopItemWidth();
                        ImGui::SameLine();
                        if (ImGui::Button(std::string("X##" + std::to_string(i)).c_str()))
                        {
                            if (m_queryParams.size() > 1)
                            {
                                m_queryParams.erase(m_queryParams.begin() + i);
                                continue;
                            }
                        }
                    }
                    m_queryParams.at(i) = q;
                }
            }
            if (ImGui::Button("Add"))
            {
                m_queryParams.push_back(std::pair<std::string, std::string>("", ""));
            }

            urlPreview.clear();

            if (!m_requestURL.empty())
            {
                urlPreview = m_requestURL + "?";
                for (auto [fst, snd] : m_queryParams)
                {
                    urlPreview.append(fst + "=" + snd + "&");
                }
                if (urlPreview.ends_with("&"))
                {
                    urlPreview.pop_back();
                }
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Header"))
        {
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

RequestWindow& RequestWindow::Instance()
{
    static RequestWindow it;
    return it;
}

void RequestWindow::SetRequest(const Request& request)
{
    m_currentRequest = request;
    m_requestType = request.type;
    m_requestURL = request.url;
}
