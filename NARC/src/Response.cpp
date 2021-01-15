#include "Response.h"

TextEditor EDITOR;

Response::Response()
{
    EDITOR.SetReadOnly(true);
    EDITOR.SetLanguageDefinition(TextEditor::LanguageDefinition::JSON());
    EDITOR.SetShowWhitespaces(false);
}

void Response::Draw() const
{
    ImGui::Begin("Response");
    //ImGui::InputTextMultiline("", JsonResponse.data(), JsonResponse.size(), ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);
    EDITOR.Render("Text Editor");
    ImGui::End();
}

void Response::SetJSON(const std::string data)
{
    JsonResponse = data;
    EDITOR.SetText(JsonResponse);
}
