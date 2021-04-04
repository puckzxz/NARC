#include "WebsocketWindow.h"

#include "App.h"
#include "imgui.h"

#include "imgui_stdlib.h"

WebsocketWindow::WebsocketWindow()
{
    m_name = "WebSocket";
    m_webSocket.disableAutomaticReconnection();
    Visible = false;
}

void WebsocketWindow::Draw()
{
    ImGui::Begin("WebSocket", &Visible, ImGuiWindowFlags_AlwaysAutoResize);

    static std::string webSocketURL = "wss://echo.websocket.org";

    static std::string webSocketMsg = "";

    ImGui::InputText("###WSURL", &webSocketURL);

    ImGui::InputText("###WSMSG", &webSocketMsg);

    if (!m_connected)
    {
        if (ImGui::Button("Connect"))
        {
            m_webSocket.setUrl(webSocketURL);
            m_webSocket.setOnMessageCallback([&](const ix::WebSocketMessagePtr& msg)
                {
                    if (msg->type == ix::WebSocketMessageType::Message)
                    {
                        m_messages.push_back(std::string("[RECEIVED] " + msg->str));
                    }
                    else if (msg->type == ix::WebSocketMessageType::Open)
                    {
                        m_messages.push_back("[CONNECTION ESTABLISHED]");
                    }
                }
            );
            m_webSocket.start();
            m_connected = true;
        }
    }
    else
    {
        if (ImGui::Button("Disconnect"))
        {
            m_webSocket.close();
            m_messages.push_back("[DISCONNECTED]");
            m_connected = false;
        }
    }

    if (m_connected)
    {
        if (ImGui::Button("Send") && !webSocketMsg.empty())
        {
            m_webSocket.send(webSocketMsg);
            m_messages.push_back("[SENT] " + webSocketMsg);
        }
        ImGui::SameLine();
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        m_messages.clear();
    }

    for (const auto& m : m_messages)
    {
        ImGui::TextUnformatted(m.c_str());
    }

    ImGui::End();
}

WebsocketWindow* WebsocketWindow::Instance()
{
    static WebsocketWindow it;
    return &it;
}
