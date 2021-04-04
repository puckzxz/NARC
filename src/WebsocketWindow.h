#pragma once

#include <string>
#include <vector>
#include <ixwebsocket/IXWebSocket.h>
#include "Window.h"

class WebsocketWindow final : public Window
{
public:
    void Draw() override;
    static WebsocketWindow* Instance();

private:
    WebsocketWindow();
    std::vector<std::string> m_messages;
    ix::WebSocket m_webSocket;
    bool m_connected = false;
};
