#include <Windows.h>

#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    if (!Engine::getInstance().Init(hInstance, 800, 600, L"WindowClass", L"Engine", nCmdShow))
    {
        return -1;
    }

    while (Engine::getInstance().ProcessMessages())
    {
        Engine::getInstance().Update();
    }    

    return 0;
}
