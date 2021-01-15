#include "Request.h"

void Request::Draw() const
{
    ImGui::Begin("Request");
    if (ImGui::Button("Send Request"))
    {
        std::thread{[]() -> bool
        {
            std::cout << "Trying to get..." << std::endl;
            const auto resp = cpr::Get(cpr::Url{"https://reqres.in/api/users/2"});
            std::cout << "Got response code: " << resp.status_code << std::endl;
            std::cout << resp.text << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds{5});
            std::cout << "Done sleeping" << std::endl;
            return resp.status_code == 200;
        }}.detach();
    }
    ImGui::End();
}
