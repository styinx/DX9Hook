#include "DX9.hpp"

bool DX9::createDevice(HWND hWnd)
{
    m_d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp{};

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed      = TRUE;
    d3dpp.SwapEffect    = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;

    m_d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &m_device);

    return true;
}

void DX9::cleanupDevice()
{
    if(m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }

    if(m_d3d)
    {
        m_d3d->Release();
        m_d3d = nullptr;
    }
}

IDirect3DDevice9* DX9::getDevice()
{
    return m_device;
}
