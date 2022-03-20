#ifndef DX11_HPP
#define DX11_HPP

#include <d3d9.h>
#include <windows.h>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class DX9
{
private:
    IDirect3D9*       m_d3d    = nullptr;
    IDirect3DDevice9* m_device = nullptr;

public:
    bool createDevice(HWND hWnd);
    void cleanupDevice();

    IDirect3DDevice9* getDevice();
};

#endif  // DX11_HPP
