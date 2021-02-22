#pragma once

#include <string>
#include <vector>
#include <ixwebsocket/IXWebSocket.h>

class WebsocketWindow
{
public:
    static WebsocketWindow& Instance();
    void Draw();
private:
    WebsocketWindow();
    std::vector<std::string> m_messages;
    ix::WebSocket m_webSocket;
    bool m_connected = false;
};
