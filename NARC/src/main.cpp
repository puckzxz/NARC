#include "App.h"

int main()
{
    std::unique_ptr<App> app = std::make_unique<App>();

    if (!app->Init())
    {
        return -1;
    }

    app->Run();
}
