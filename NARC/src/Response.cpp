#include "Response.h"

void Response::Draw() const
{
    ImGui::Begin("Response");
    ImGui::InputTextMultiline("", JsonResponse.data(), JsonResponse.size(), ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);
    ImGui::End();
}
