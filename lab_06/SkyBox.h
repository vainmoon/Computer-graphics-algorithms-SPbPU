#pragma once

#include "Shapes.h"

class SkyBox : public Shape
{
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
		DirectX::XMVECTOR radius;
	};

	struct Vertex
	{
		float x, y, z;
	};
public:
	HRESULT CreateGeometry(ID3D11Device* m_pDevice) final;
	HRESULT CreateShaders(ID3D11Device* m_pDevice) final;
	HRESULT CreateTextures(ID3D11Device* m_pDevice) final;
	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext) final;

	void setCamPos(DirectX::XMVECTOR camPos);
	void setRadius(const float& fov, const float& nearPlane, const float& width, const float& height);

private:
	GeomBuffer geomBuffer;
	int numIndeces;
	DirectX::XMVECTOR camPos;
	float radius;
};

