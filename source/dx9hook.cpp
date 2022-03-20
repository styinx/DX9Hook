#include "dx9hook.hpp"

#include <d3d9.h>

DXHook hook{};

HRESULT __stdcall EndSceneHook(IDirect3DDevice9* device)
{
    if(!hook.ready)
    {
        std::cout << "Init ImGui" << std::endl;

        ImGui::CreateContext();
        ImGui_ImplWin32_Init(hook.target_window);
        ImGui_ImplDX9_Init(device);

        hook.ready = true;
    }
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    bool bShow = true;
    ImGui::ShowDemoWindow(&bShow);

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return originalEndScene(device);
}

void initHook()
{
    auto windowed = ((GetWindowLongPtr(hook.target_window, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;

    D3DPRESENT_PARAMETERS d3dpp = D3DPRESENT_PARAMETERS{};
    IDirect3D9*           d3d   = Direct3DCreate9(D3D_SDK_VERSION);

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.BackBufferCount            = 1;
    d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
    d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow              = hook.target_window;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;
    d3dpp.Windowed                   = windowed;

    d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hook.target_window,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &hook.device);

    hook.device_vtable = (DWORD_PTR*)(hook.device);
    hook.device_vtable = (DWORD_PTR*)hook.device_vtable[0];

    initMinHook();
}

void shutdownHook()
{
    hook.device->Release();

    shutdownMinHook();

    FreeLibraryAndExitThread(hook.target_module, 0);
}

UINT initMinHook()
{
    if(MH_Initialize() != MH_OK)
    {
        return 1;
    }

    if(MH_CreateHook(
           (DWORD_PTR*)hook.device_vtable[42],
           (DWORD_PTR*)EndSceneHook,
           reinterpret_cast<void**>(&originalEndScene)) != MH_OK)
    {
        return 1;
    }

    if(MH_EnableHook((DWORD_PTR*)hook.device_vtable[42]) != MH_OK)
    {
        return 1;
    }
    return 0;
}

void shutdownMinHook()
{
    if(MH_DisableHook(MH_ALL_HOOKS))
        return;
    if(MH_Uninitialize())
        return;
}
