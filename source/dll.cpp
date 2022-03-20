#include "dx9hook.hpp"
#include "imgui.h"

#include <iostream>
#include <windows.h>

static WNDPROC windowProcessHandler = nullptr;

extern IMGUI_IMPL_API LRESULT
ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK hookWindowProcessHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam) > 0)
        return 1L;

    return ::CallWindowProcA(windowProcessHandler, hwnd, msg, wParam, lParam);
}

DWORD WINAPI mainThread(LPVOID p)
{
    auto module = reinterpret_cast<HMODULE>(p);
    if(!module)
    {
        std::cout << "Invalid Module" << std::endl;
        return -1;
    }

    auto window = reinterpret_cast<HWND>(FindWindow(nullptr, app_title));
    if(!window)
    {
        std::cout << "Invalid Window" << std::endl;
        return -1;
    }

    hook.target_window = window;
    hook.target_module = module;
    initHook();

    windowProcessHandler =
        (WNDPROC)::SetWindowLongPtr(window, GWLP_WNDPROC, (LRESULT)hookWindowProcessHandler);

    return S_OK;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    switch(reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        std::cout << "PROCESS ATTACH" << std::endl;

        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);

        HANDLE hThread = CreateThread(nullptr, 0, &mainThread, module, 0, nullptr);
        if(hThread != nullptr)
        {
            std::cout << "Start hook thread" << std::endl;
            CloseHandle(hThread);
        }
        break;
    }
    case DLL_THREAD_ATTACH:
        std::cout << "THREAD ATTACH" << std::endl;
        break;
    case DLL_THREAD_DETACH:
        std::cout << "THREAD DETACH" << std::endl;
        break;
    case DLL_PROCESS_DETACH:
    {
        std::cout << "PROCESS DETACH" << std::endl;
        shutdownHook();
        break;
    }
    default:
        std::cout << "DEFAULT" << std::endl;
        break;
    }

    return TRUE;
}