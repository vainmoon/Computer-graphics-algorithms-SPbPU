#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

struct Vertex
{
    float x, y, z;
    COLORREF color;
};

class Renderer {
public:
    bool InitDevice(HWND hWnd);
    bool Render();
    bool Resize(UINT width, UINT height);
    void ReleaseDevice();

private:
    HRESULT SetupBackBuffer();
    HRESULT InitScene();

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;
    IDXGISwapChain* m_pSwapChain = nullptr;
    ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;
    ID3D11Buffer* m_pVertexBuffer = nullptr;
    ID3D11Buffer* m_pIndexBuffer = nullptr;
    ID3D11InputLayout* m_pInputLayout = nullptr;
    ID3D11VertexShader* m_pVertexShader = nullptr;
    ID3D11PixelShader* m_pPixelShader = nullptr;
    UINT m_width = 0;
    UINT m_height = 0;
};