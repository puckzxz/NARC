#include "Request.h"

#include <array>

void Request::Draw() const
{
    ImGui::Begin("Request");
    // TODO: Convert to std::string
    ImGui::InputText("Request URL", _requestUrlBuffer.data(), _requestUrlBuffer.max_size());
    ImGui::SameLine();
    if (ImGui::Button("Send Request"))
    {
        std::thread{[&]() -> bool
        {
            const std::string requestUrl(_requestUrlBuffer.begin(), _requestUrlBuffer.end());
            std::cout << "Trying to get: " << requestUrl << std::endl;
            const auto resp = cpr::Get(cpr::Url{requestUrl});
            std::cout << "Got response code: " << resp.status_code << std::endl;
            std::cout << resp.text << std::endl;
            std::cout << json::parse(resp.text).dump(4) << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds{5});
            std::cout << "Done sleeping" << std::endl;
            return resp.status_code == 200;
        }}.detach();
    }
    ImGui::End();
}
