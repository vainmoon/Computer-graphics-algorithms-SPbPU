#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "Shapes.h"

#pragma comment(lib, "d3d11")

class Light
{
	struct PointLight
	{
		float pos[4];
		float color[4];
	};

	struct SceneBuffer
	{
		DirectX::XMMATRIX vp;
		DirectX::XMVECTOR cameraPos;
	};

	struct LightBuffer
	{
		PointLight lightsDesc[10];
		float ambientColor[4]; // (r,g,b,a): a - intensity, rgb - color
		int lightsCount;
		float diffuseCoef;
		float specularCoef;
		float shine;
	};
public:
	Light() = default;
	~Light() = default;
	HRESULT createBuffer(ID3D11Device* device);
	void updateBuffer(ID3D11DeviceContext* m_pDeviceContext);
	void setAmbient(float r, float g, float b, float intensity);
	void RenderImGUI(ID3D11Device* device, ID3D11DeviceContext* m_pDeviceContext);
	void addLight(ID3D11Device* device, ID3D11DeviceContext* m_pDeviceContext);

	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext);

	void setCamPos(DirectX::XMVECTOR camPos);

	void Clean();
private:
	LightBuffer light;
	ID3D11Buffer* lightBuffer;

	SceneBuffer scBuffer;
	ID3D11Buffer* sceneBuffer;
	
	std::vector<Sphere> lightsShapes;
};

