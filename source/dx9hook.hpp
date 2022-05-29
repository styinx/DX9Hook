#ifndef D3D9_HOOK_H_INCLUDED_
#define D3D9_HOOK_H_INCLUDED_

#include "MinHook.h"
#include "imgui.h"
#include "backends/imgui_impl_dx9.h"
#include "backends/imgui_impl_win32.h"

#include <d3d9.h>
#include <fstream>
#include <iostream>
#include <windows.h>

/**
 * DX9 Stuff
 */

typedef HRESULT(__stdcall* EndScene)(IDirect3DDevice9*);

static EndScene originalEndScene = nullptr;

/**
 * Hook Stuff
 */

void initHook();
void shutdownHook();

UINT initMinHook();
void shutdownMinHook();

struct DXHook
{
    bool    ready         = false;
    bool    draw          = true;
    HMODULE target_module = nullptr;
    HWND    target_window = nullptr;

    IDirect3DDevice9* device = nullptr;

    DWORD_PTR* device_vtable = nullptr;
};

extern DXHook      hook;
extern const char* app_title;

#endif  // D3D9_HOOK_H_INCLUDED_