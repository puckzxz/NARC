#include "RequestWindow.h"

void RequestWindow::Draw()
{
    ImGui::Begin("Request");
    // static std::array<char, 4096> requestUrlBuffer;
    static const std::array<std::string, 7> requestTypes = {
        "GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"
    };
    static std::string requestName = requestTypes[m_requestTypeIndex];
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::BeginCombo("###RequestType", requestTypes.at(m_requestTypeIndex).c_str()))
    {
        for (auto i = 0; i < requestTypes.size(); i++)
        {
            const auto is_selected = (m_requestTypeIndex == i);
            if (ImGui::Selectable(requestTypes.at(i).c_str()))
            {
                m_requestTypeIndex = i;
                requestName = requestTypes[i];
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::InputText("###RequestURL", m_requestUrlBuffer.data(), m_requestUrlBuffer.size());
    ImGui::PopItemWidth();
    if (ImGui::Button("Send"))
    {
        std::thread{
            [&]() -> void
            {
                const std::string requestUrl = m_requestUrlBuffer.data();
                if (requestUrl.length() <= 0)
                {
                    return;
                }
                cpr::Response resp;
                if (requestName == "GET")
                    resp = cpr::Get(cpr::Url{requestUrl});
                else if (requestName == "POST")
                    resp = cpr::Post(cpr::Url{requestUrl});
                else if (requestName == "PUT")
                    resp = cpr::Put(cpr::Url{requestUrl});
                else if (requestName == "PATCH")
                    resp = cpr::Patch(cpr::Url{requestUrl});
                else if (requestName == "DELETE")
                    resp = cpr::Delete(cpr::Url{requestUrl});
                else if (requestName == "OPTIONS")
                    resp = cpr::Options(cpr::Url{requestUrl});
                else if (requestName == "HEAD")
                    resp = cpr::Head(cpr::Url{requestUrl});
                else
                    throw std::exception("Tried to send a request with no method selected");
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
}
