#include "RequestWindow.h"

void Request::Draw() const
{
    ImGui::Begin("Request");
    static std::array<char, 4096> requestUrlBuffer;
    static auto requestTypeIndex = 0;
    static const std::array<std::string, 7> requestTypes = {
        "GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"
    };
    static std::string requestName = requestTypes[requestTypeIndex];
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::BeginCombo("###RequestType", requestTypes.at(requestTypeIndex).c_str()))
    {
        for (auto i = 0; i < requestTypes.size(); i++)
        {
            const auto is_selected = (requestTypeIndex == i);
            if (ImGui::Selectable(requestTypes.at(i).c_str()))
            {
                requestTypeIndex = i;
                requestName = requestTypes[i];
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::InputText("###RequestURL", requestUrlBuffer.data(), requestUrlBuffer.size());
    ImGui::PopItemWidth();
    if (ImGui::Button("Send"))
    {
        std::thread{
            [&]() -> void
            {
                const std::string requestUrl = requestUrlBuffer.data();
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
                ResponseWindow::Get().SetResponseHeaders(resp.header);
                ResponseWindow::Get().SetResponseDuration(resp.elapsed);
                ResponseWindow::Get().SetResponseCode(resp.status_code);
                if (!resp.text.empty())
                    ResponseWindow::Get().SetJSON(json::parse(resp.text).dump(4));
            }
        }.detach();
    }
    ImGui::End();
}

Request& Request::Get()
{
    static Request s_Request;
    return s_Request;
}
