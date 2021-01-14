#include "App.h"

int main()
{
    auto app = std::make_unique<App>();

    if (!app->Init())
    {
        return -1;
    }

    app->Run();
}
