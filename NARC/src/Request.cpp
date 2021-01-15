#include "Request.h"

void Request::Draw() const
{
    ImGui::Begin("Request");
    if (ImGui::Button("Send Request"))
    {
        std::cout << "Trying to get..." << std::endl;
        cpr::Response r = cpr::Get(cpr::Url{"https://google.com"});
        std::cout << "Got response code: " << r.status_code << std::endl;
    }
    ImGui::End();
}
