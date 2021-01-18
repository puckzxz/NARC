#include "Request.h"

void Request::Draw() const
{
    ImGui::Begin("Request");
    // TODO: Convert to std::string
    static std::array<char, 4096> requestUrlBuffer;
    static  int _requestTypeIndex = 0;
    static  const std::array<std::string, 7> _requestTypes = {"GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"};
    ImGui::PushItemWidth(-FLT_MIN);
    if(ImGui::BeginCombo("###RequestType", _requestTypes.at(_requestTypeIndex).c_str()))
    {
        for (int i = 0; i < _requestTypes.size(); i++)
        {
            const bool is_selected = (_requestTypeIndex == i);
            if(ImGui::Selectable(_requestTypes.at(i).c_str()))
            {
                _requestTypeIndex = i;
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
        std::thread{[&]() -> void
        {
            const std::string requestUrl = requestUrlBuffer.data();
            if (requestUrl.length() <= 0)
            {
                return;
            }
            std::cout << "Trying to get: " << requestUrl << std::endl;
            const auto resp = cpr::Get(cpr::Url{requestUrl});
            std::cout << "Got response code: " << resp.status_code << std::endl;
            Response::SetResponseCode(resp.status_code);
            Response::SetJSON(json::parse(resp.text).dump(4));
        }}.detach();
    }
    ImGui::End();
}
