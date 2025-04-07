#pragma once

#include "Camera.h"
#include "Shapes.h"
#include "Light.h"
#include "SkyBox.h"

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment( lib, "dxguid.lib")

class Graphics
{
public:
	~Graphics();

	bool InitDirectX(HWND hwnd, int width, int height);
	bool InitShaders();
	bool InitScene();
	void InitImGUI(HWND hwnd);

	void RenderFrame();
	//void RenderImGUI();

	Camera& GetCamera();

	void Resize(const int& width, const int& height);

	HRESULT CreateDepthBuffer();
	HRESULT CreateBlendState();
private:
	int windowWidth;
	int windowHeight;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;

	ID3D11Texture2D* m_pDepthBuffer;
	ID3D11DepthStencilView* m_pDepthBufferDSV;
	ID3D11DepthStencilState* m_pDepthState;
	ID3D11DepthStencilState* m_pDepthTransparentState;

	ID3D11BlendState* m_pTransBlendState;

	Camera camera;
	Cube cube;
	Cube cube1;
	SkyBox skyBox;
	Square square;
	Square square1;
	Light light;
};

