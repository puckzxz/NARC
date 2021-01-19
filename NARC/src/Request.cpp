﻿#include "Request.h"

static Request s_Request;

void Request::Draw() const
{
    ImGui::Begin("Request");
    static std::array<char, 4096> requestUrlBuffer;
    static auto requestTypeIndex = 0;
    static const std::array<std::string, 7> requestTypes = {
        "GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"
    };
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::BeginCombo("###RequestType", requestTypes.at(requestTypeIndex).c_str()))
    {
        for (auto i = 0; i < requestTypes.size(); i++)
        {
            const auto is_selected = (requestTypeIndex == i);
            if (ImGui::Selectable(requestTypes.at(i).c_str()))
            {
                requestTypeIndex = i;
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
                std::cout << "Trying to get: " << requestUrl << std::endl;
                const auto resp = cpr::Get(cpr::Url{requestUrl});
                std::cout << "Got response code: " << resp.status_code << std::endl;
                Response::Get().SetResponseCode(resp.status_code);
                Response::Get().SetJSON(json::parse(resp.text).dump(4));
            }
        }.detach();
    }
    ImGui::End();
}

Request& Request::Get()
{
    return s_Request;
}
