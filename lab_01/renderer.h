#pragma once

#include <d3d11.h>
#include <dxgi.h>

class Renderer {
public:
    bool InitDevice(HWND hWnd);
    bool Render();
    bool Resize(UINT width, UINT height);

private:
    HRESULT SetupBackBuffer();

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;
    IDXGISwapChain* m_pSwapChain = nullptr;
    ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;
    UINT m_width = 0;
    UINT m_height = 0;
};