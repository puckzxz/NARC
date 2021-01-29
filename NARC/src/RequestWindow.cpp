#include "RequestWindow.h"

#include "misc/cpp/imgui_stdlib.h"
#include "misc/cpp/imgui_stdlib.cpp"

void RequestWindow::Draw()
{
    ImGui::Begin("Request", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
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
                    resp = cpr::Get(cpr::Url{m_requestURL});
                else if (m_requestName == "POST")
                    resp = cpr::Post(cpr::Url{m_requestURL});
                else if (m_requestName == "PUT")
                    resp = cpr::Put(cpr::Url{m_requestURL});
                else if (m_requestName == "PATCH")
                    resp = cpr::Patch(cpr::Url{m_requestURL});
                else if (m_requestName == "DELETE")
                    resp = cpr::Delete(cpr::Url{m_requestURL});
                else if (m_requestName == "OPTIONS")
                    resp = cpr::Options(cpr::Url{m_requestURL});
                else if (m_requestName == "HEAD")
                    resp = cpr::Head(cpr::Url{m_requestURL});
                else
                    throw std::exception("Unknown REST Method");
                ResponseWindow::Instance().SetResponse(resp);
            }
        }.detach();
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
