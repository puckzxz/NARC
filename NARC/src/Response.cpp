#include "Response.h"

#include "Request.h"

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
    if (ResponseCode >= 100 && ResponseCode <= 199)
        ImGui::TextColored({150, 150, 150, 1}, "%d", ResponseCode);
    if (ResponseCode >= 200 && ResponseCode <= 299)
        ImGui::TextColored({0, 255, 0, 1}, "%d", ResponseCode);
    if (ResponseCode >= 300 && ResponseCode <= 399)
        ImGui::TextColored({255, 255, 0, 1}, "%d", ResponseCode);
    if (ResponseCode >= 400 && ResponseCode <= 499)
        ImGui::TextColored({255, 165, 0, 1}, "%d", ResponseCode);
    if (ResponseCode >= 500 && ResponseCode <= 599)
        ImGui::TextColored({255, 0, 0, 1}, "%d", ResponseCode);
    EDITOR.Render("Text Editor");
    ImGui::End();
}

void Response::SetJSON(const std::string& data)
{
    JsonResponse = data;
    EDITOR.SetText(JsonResponse);
}

void Response::SetResponseCode(const int32_t& code)
{
    ResponseCode = code;
}
