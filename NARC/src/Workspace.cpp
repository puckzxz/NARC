#include "Workspace.h"

Workspace s_Workspace;

void Workspace::Draw() const
{
    // TODO: Fix default item, refer to Request.cpp
    static std::string comboName = "Workspace 1";
    ImGui::Begin("Workspace");
    if(ImGui::BeginCombo("###Workspace", comboName.c_str()))
    {
        for (int i = 1; i < 6; i++)
        {
            std::string itemName = "Workspace " + std::to_string(i);
            if(ImGui::Selectable(itemName.c_str()))
            {
                comboName = itemName;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::End();
}

Workspace& Workspace::Get()
{
    return s_Workspace;
}
