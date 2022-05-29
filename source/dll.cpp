#include "dx9hook.hpp"

#include "imgui.h"

#include <iostream>
#include <windows.h>

static WNDPROC windowProcessHandler = nullptr;
const char* app_title = "Demo";

extern IMGUI_IMPL_API LRESULT
ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK hookWindowProcessHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam) > 0)
        return 1L;

    return CallWindowProcA(windowProcessHandler, hwnd, msg, wParam, lParam);
}

DWORD WINAPI mainThread(LPVOID p)
{
    auto module = reinterpret_cast<HMODULE>(p);
    if(!module)
    {
        return -1;
    }

    auto window = reinterpret_cast<HWND>(FindWindow(nullptr, app_title));
    if(!window)
    {
        return -1;
    }

    hook.target_window = window;
    hook.target_module = module;
    initHook();

    windowProcessHandler =
        (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LRESULT)hookWindowProcessHandler);

    return S_OK;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    switch(reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE hThread = CreateThread(nullptr, 0, &mainThread, module, 0, nullptr);
        if(hThread != nullptr)
        {
            CloseHandle(hThread);
        }
        break;
    }
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
    {
        shutdownHook();
        break;
    }
    default:
        break;
    }

    return TRUE;
}