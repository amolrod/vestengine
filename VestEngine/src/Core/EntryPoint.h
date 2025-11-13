#pragma once

#ifdef VEST_PLATFORM_WINDOWS
#include <windows.h>
#endif

#include "Core/Application.h"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    Vest::Application* app = Vest::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
