#pragma once

#include <string>

class Window
{
public:
    virtual void Draw() = 0;
    bool Visible = true;
    inline const std::string& Name() const { return m_name; };
    virtual ~Window() = default;

protected:
    std::string m_name = "Unnamed";
};
