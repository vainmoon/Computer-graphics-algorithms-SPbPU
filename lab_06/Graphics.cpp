#include "Graphics.h"
#include "DDSTextureLoader11.h"

#include <assert.h>
#include <string>

#define SAFE_RELEASE(p) if (p != NULL) { p->Release(); p = NULL; }

Graphics::~Graphics()
{
    cube.Clean();
    cube1.Clean();
    skyBox.Clean();
    square.Clean();
    square1.Clean();
    light.Clean();
    SAFE_RELEASE(m_pBackBufferRTV);
    SAFE_RELEASE(m_pSwapChain);
    SAFE_RELEASE(m_pDeviceContext);
    SAFE_RELEASE(m_pDepthBuffer);
    SAFE_RELEASE(m_pDepthBufferDSV);
    SAFE_RELEASE(m_pDepthState);
    SAFE_RELEASE(m_pDepthTransparentState);
    SAFE_RELEASE(m_pTransBlendState);

    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX11_Shutdown();

    ID3D11Debug* d3Debug = nullptr;
    m_pDevice->QueryInterface(IID_PPV_ARGS(&d3Debug));

    UINT references = m_pDevice->Release();
    if (references > 1)
    {
        d3Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    }

    SAFE_RELEASE(d3Debug);
}

bool Graphics::InitDirectX(HWND hwnd, int width, int height)
{
    windowHeight = height;
    windowWidth = width;

    HRESULT result;

    IDXGIFactory* pFactory = NULL;
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
    assert(SUCCEEDED(result));

    IDXGIAdapter* pSelectedAdapter = NULL;

    if (SUCCEEDED(result))
    {
        IDXGIAdapter* pAdapter = NULL;
        UINT adapterIdx = 0;

        while (SUCCEEDED(pFactory->EnumAdapters(adapterIdx, &pAdapter)))
        {
            DXGI_ADAPTER_DESC desc;

            pAdapter->GetDesc(&desc);

            if (wcscmp(desc.Description, L"Microsoft Basic Render Driver") != 0)
            {
                pSelectedAdapter = pAdapter;
                break;
            }

            pAdapter->Release();
            adapterIdx++;
        }
    }
    assert(pSelectedAdapter != NULL);

    D3D_FEATURE_LEVEL level;
    D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };

    if (SUCCEEDED(result))
    {
        UINT flags = 0;
#ifdef _DEBUG
        flags = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

        result = D3D11CreateDevice(pSelectedAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL,
            flags, levels, 1, D3D11_SDK_VERSION, &m_pDevice, &level, &m_pDeviceContext);
    }

    if (SUCCEEDED(result))
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
        swapChainDesc.BufferCount = 2;
        swapChainDesc.BufferDesc.Width = width;
        swapChainDesc.BufferDesc.Height = height;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hwnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Windowed = true;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.Flags = 0;

        result = pFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
    }

    if (SUCCEEDED(result))
    {
        ID3D11Texture2D* pBackBuffer = NULL;
        result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        if (SUCCEEDED(result))
        {
            result = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBufferRTV);

            SAFE_RELEASE(pBackBuffer);
        }
    }

    if (SUCCEEDED(result))
    {
        result = CreateDepthBuffer();
    }

    if (SUCCEEDED(result))
    {
        result = CreateBlendState();
    }

    ID3D11RenderTargetView* views[] = { m_pBackBufferRTV };
    m_pDeviceContext->OMSetRenderTargets(1, views, m_pDepthBufferDSV);

    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)width;
    viewport.Height = (FLOAT)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_pDeviceContext->RSSetViewports(1, &viewport);

    if (SUCCEEDED(result))
    {
        result = cube.setRasterizerState(m_pDevice, D3D11_CULL_MODE::D3D11_CULL_BACK);
        result = cube1.setRasterizerState(m_pDevice, D3D11_CULL_MODE::D3D11_CULL_BACK);
        result = skyBox.setRasterizerState(m_pDevice, D3D11_CULL_MODE::D3D11_CULL_FRONT);
    }
    if (SUCCEEDED(result))
        result = square.setRasterizerState(m_pDevice, D3D11_CULL_MODE::D3D11_CULL_NONE);
    if (SUCCEEDED(result))
        result = square1.setRasterizerState(m_pDevice, D3D11_CULL_MODE::D3D11_CULL_NONE);

    cube.CreateTextures(m_pDevice);
    cube1.CreateTextures(m_pDevice);
    skyBox.CreateTextures(m_pDevice);
    
    SAFE_RELEASE(pSelectedAdapter);
    SAFE_RELEASE(pFactory);

    return SUCCEEDED(result);
}

HRESULT SetResourceName(ID3D11DeviceChild* pResource, const std::string name)
{
    return pResource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
}

bool Graphics::InitShaders()
{
    HRESULT result = cube.CreateShaders(m_pDevice);
    result = cube1.CreateShaders(m_pDevice);
    result = square.CreateShaders(m_pDevice);
    result = square1.CreateShaders(m_pDevice);
    result = light.createBuffer(m_pDevice);

    if (SUCCEEDED(result))
    {
        result = skyBox.CreateShaders(m_pDevice);
    }

    return SUCCEEDED(result);
}

bool Graphics::InitScene()
{
    HRESULT hr = cube.CreateGeometry(m_pDevice);
    hr = cube1.CreateGeometry(m_pDevice);
    hr = square.CreateGeometry(m_pDevice);
    hr = square1.CreateGeometry(m_pDevice);

    if (SUCCEEDED(hr))
    {
        hr = skyBox.CreateGeometry(m_pDevice);
    }

    camera.SetPosition(DirectX::XMVectorSet(-2.0f, 0.0f, 0.0f, 0.0));
    camera.SetProjectionValues(100.0f, (float)windowHeight / windowWidth, 0.1f, 100.0f);
    camera.AdjustRotation(DirectX::XMVectorSet(0.0f, DirectX::XM_PIDIV2, 0.0f, 1.0f));

    skyBox.setRadius(camera.GetFov(), camera.GetNearPlane(), static_cast<float>(windowWidth), static_cast<float>(windowHeight));
    
    square.setColor(DirectX::XMVectorSet(1.0f, 0.0f, 1.0f, 0.5f));
    square1.setColor(DirectX::XMVectorSet(0.0f, 1.0f, 0.5f, 0.5f));

    light.setAmbient(0.5f, 0.5f, 0.5f, 0.5f);

    light.addLight(m_pDevice, m_pDeviceContext);

    return SUCCEEDED(hr);
}

void Graphics::InitImGUI(HWND hwnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext);
    ImGui::StyleColorsDark();
}

void Graphics::RenderFrame()
{
    m_pDeviceContext->ClearState();

    ID3D11RenderTargetView* views[] = { m_pBackBufferRTV };
    m_pDeviceContext->OMSetRenderTargets(1, views, m_pDepthBufferDSV);

    static const FLOAT BackColor[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
    m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV, BackColor);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthBufferDSV, D3D11_CLEAR_DEPTH, 0.0f, 0);

    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)windowWidth;
    viewport.Height = (FLOAT)windowHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_pDeviceContext->RSSetViewports(1, &viewport);

    D3D11_RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = windowWidth;
    rect.bottom = windowHeight;
    m_pDeviceContext->RSSetScissorRects(1, &rect);

    m_pDeviceContext->OMSetDepthStencilState(m_pDepthState, 0);

    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    DirectX::XMMATRIX vp = camera.GetViewMatrix()* camera.GetProjectionMatrix();
    cube.Translate(DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.7f));

    skyBox.setCamPos(camera.GetPositionVector());
    light.setCamPos(camera.GetPositionVector());

    square.Rotate(DirectX::XMMatrixRotationX(DirectX::XM_PIDIV2));
    square.Translate(DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.5f));
    square1.Rotate(DirectX::XMMatrixRotationX(DirectX::XM_PIDIV2));
    square1.Translate(DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.3f));

    light.updateBuffer(m_pDeviceContext);
    cube.Draw(vp, m_pDeviceContext);
    cube1.Draw(vp, m_pDeviceContext);
    light.Draw(vp, m_pDeviceContext);
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthTransparentState, 0);
    skyBox.Draw(vp, m_pDeviceContext);
    
    m_pDeviceContext->OMSetBlendState(m_pTransBlendState, nullptr, 0xFFFFFFFF);
    square.Draw(vp, m_pDeviceContext);
    square1.Draw(vp, m_pDeviceContext);

    light.RenderImGUI(m_pDevice, m_pDeviceContext);

    HRESULT result = m_pSwapChain->Present(0, 0);
    assert(SUCCEEDED(result));
}

//void Graphics::RenderImGUI()
//{
//    ImGui_ImplWin32_NewFrame();
//    ImGui_ImplDX11_NewFrame();
//    ImGui::NewFrame();
//
//    ImGui::Begin("Test");
//    ImGui::End();
//
//    ImGui::Render();
//
//    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//}

Camera& Graphics::GetCamera()
{
    return camera;
}

void Graphics::Resize(const int& width, const int& height)
{
    if ((width != windowWidth || height != windowHeight) && m_pSwapChain != nullptr)
    {
        if (m_pBackBufferRTV != NULL)
        {
            m_pBackBufferRTV->Release();
            m_pBackBufferRTV = NULL;
        }

        if (m_pDepthBufferDSV!= NULL)
        {
            m_pDepthBufferDSV->Release();
            m_pDepthBufferDSV = NULL;
        }

        HRESULT result = m_pSwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
        if (SUCCEEDED(result))
        {
            windowWidth = width;
            windowHeight = height;

            ID3D11Texture2D* pBackBuffer = NULL;
            HRESULT result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            if (SUCCEEDED(result))
            {
                result = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBufferRTV);

                if (pBackBuffer != NULL)
                {
                    pBackBuffer->Release();
                    pBackBuffer = NULL;
                }
            }

            skyBox.setRadius(camera.GetFov(), camera.GetNearPlane(), static_cast<float>(windowWidth), static_cast<float>(windowHeight));

            assert(SUCCEEDED(result));
        }

        if (SUCCEEDED(result))
        {
            CreateDepthBuffer();
        }
    }
}

HRESULT Graphics::CreateDepthBuffer()
{
    HRESULT result;

    D3D11_TEXTURE2D_DESC desc;

    desc.Format = DXGI_FORMAT_D32_FLOAT;
    desc.ArraySize = 1;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.Height = windowHeight;
    desc.Width = windowWidth;
    desc.MipLevels = 1;

    result = m_pDevice->CreateTexture2D(&desc, nullptr, &m_pDepthBuffer);
    if (SUCCEEDED(result))
    {
        result = m_pDevice->CreateDepthStencilView(m_pDepthBuffer, nullptr, &m_pDepthBufferDSV);
        SAFE_RELEASE(m_pDepthBuffer);
    }

    if (SUCCEEDED(result))
    {
        D3D11_DEPTH_STENCIL_DESC desc = {};
        desc.DepthEnable = TRUE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_GREATER;
        desc.StencilEnable = FALSE;

        result = m_pDevice->CreateDepthStencilState(&desc, &m_pDepthState);

        desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        result = m_pDevice->CreateDepthStencilState(&desc, &m_pDepthTransparentState);
    }

    return result;
}

HRESULT Graphics::CreateBlendState()
{
    D3D11_BLEND_DESC desc = {};

    desc.AlphaToCoverageEnable = false;
    desc.IndependentBlendEnable = false;
    desc.RenderTarget[0].BlendEnable = true;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED |
        D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;

    HRESULT result = m_pDevice->CreateBlendState(&desc, &m_pTransBlendState);

    return result;
}
