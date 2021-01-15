#include "Request.h"

void Request::Draw() const
{
    ImGui::Begin("Request");
    // TODO: Convert to std::string
    static std::array<char, 2048> _requestUrlBuffer = {};
    static  int _requestTypeIndex = 0;
    static  const std::array<std::string, 7> _requestTypes = {"GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"};
    if(ImGui::BeginCombo("Request Type", _requestTypes.at(_requestTypeIndex).c_str()))
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
    ImGui::InputText("Request URL", _requestUrlBuffer.data(), _requestUrlBuffer.max_size());
    if (ImGui::Button("Send"))
    {
        std::thread{[&]() -> bool
        {
            const std::string requestUrl(_requestUrlBuffer.begin(), _requestUrlBuffer.end());
            if (requestUrl.length() <= 0)
            {
                return false;
            }
            std::cout << "Trying to get: " << requestUrl << std::endl;
            const auto resp = cpr::Get(cpr::Url{requestUrl});
            std::cout << "Got response code: " << resp.status_code << std::endl;
            Response::SetJSON(json::parse(resp.text).dump(4));
            return resp.status_code == 200;
        }}.detach();
    }
    ImGui::End();
}
