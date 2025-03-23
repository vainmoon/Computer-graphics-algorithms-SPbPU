#pragma once

#include "framework.h"
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <d3dcompiler.h>
#include "SceneManager.h"
#include "LoadDDS.h"

class Renderer {
public:
    SceneManager pSceneManager;

    static Renderer& GetInstance() {
        static Renderer instance;
        return instance;
    }
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    bool Init(HWND hWnd);
    HRESULT InitShaders();
    void Clean();
    bool Render();
    bool Resize(UINT width, UINT height);
    bool IsRunning() { return m_isRunning; }
    ~Renderer();
private:
    Renderer() {};
    HRESULT InitTextures();
    HRESULT CompileShader(const std::wstring& path, ID3D11DeviceChild** ppShader, const std::string& ext, ID3DBlob** ppCode = nullptr);
    HRESULT SetupBackBuffer();
    HRESULT SetupDepthBlend();
    void InitCube(ID3D11DepthStencilState* DepthState, ID3D11BlendState* BlendState, ID3D11InputLayout* InputLayout, ID3D11PixelShader* PS,
        ID3D11VertexShader* VS, ID3D11ShaderResourceView* TextureView, ID3D11Buffer* IndexBuffer, ID3D11Buffer* VertexBuffer, UINT StridesSize);

    unsigned int m_width = 1280;
    unsigned int m_height = 720;
    IDXGISwapChain* m_pSwapChain = NULL;
    ID3D11Device* m_pDevice = NULL;
    ID3D11DeviceContext* m_pDeviceContext = NULL;
    ID3D11RenderTargetView* m_pBackBufferRTV = NULL;

    ID3D11Buffer* m_pSphereVertexBuffer = NULL;
    ID3D11Buffer* m_pSphereIndexBuffer = NULL;
    ID3D11Buffer* m_pCubeVertexBuffer = NULL;
    ID3D11Buffer* m_pCubeIndexBuffer = NULL;

    ID3D11Buffer* m_pSceneBuffer = NULL;
    ID3D11Buffer* m_pViewBuffer = NULL;

    ID3D11PixelShader* m_pTexturePS = NULL;
    ID3D11VertexShader* m_pTextureVS = NULL;
    ID3D11InputLayout* m_pTextureInputLayout = NULL;

    ID3D11PixelShader* m_pSkyboxPS = NULL;
    ID3D11VertexShader* m_pSkyboxVS = NULL;
    ID3D11InputLayout* m_pSkyboxInputLayout = NULL;

    ID3D11Texture2D* m_pCubeTexture = NULL;
    ID3D11ShaderResourceView* m_pCubeTextureView = NULL;
    ID3D11SamplerState* m_pTextureSampler = NULL;

    ID3D11Texture2D* m_pCubemapTexture = NULL;
    ID3D11ShaderResourceView* m_pCubemapTextureView = NULL;

    ID3D11Texture2D* m_pDepthBuffer = NULL;
    ID3D11DepthStencilView* m_pDepthBufferDSV = NULL;
    ID3D11DepthStencilState* m_pDepthStateReadWrite = NULL;
    ID3D11DepthStencilState* m_pDepthStateRead = NULL;

    ID3D11PixelShader* m_pSimpleTransTexturePixelShader = NULL;
    ID3D11VertexShader* m_pSimpleTransTextureVertexShader = NULL;
    ID3D11InputLayout* m_pSimpleTransTextureInputLayout = NULL;

    ID3D11BlendState* m_pTransBlendState = NULL;

    HRESULT SetupDepthBuffer();

    bool m_isRunning = false;
};