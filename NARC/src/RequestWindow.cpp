#include "RequestWindow.h"

#include "misc/cpp/imgui_stdlib.h"
#include "misc/cpp/imgui_stdlib.cpp"

RequestWindow::RequestWindow()
{
    m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::JSON());
    m_editor.SetShowWhitespaces(false);
}

void RequestWindow::Draw()
{
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
                    throw std::exception("Unknown REST Method");
                ResponseWindow::Instance().SetResponse(resp);
            }
        }.detach();
    }
    ImGui::SameLine();
    if (ImGui::Button("Format"))
    {
        const auto text = m_editor.GetText();
        if (json::accept(text))
        {
            m_editor.SetText(json::parse(text).dump(4));
        }
    }
    m_editor.Render("Request Editor");
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
