#pragma once

#include "Shaders.h"

#include <d3d11.h>
#include <DirectXMath.h>

#include <vector>

#pragma comment(lib, "d3d11")

#define SAFE_RELEASE(p) if (p != NULL) { p->Release(); p = NULL; }

class Shape
{
protected:
	struct SceneBuffer
	{
		DirectX::XMMATRIX vp;
		DirectX::XMVECTOR cameraPos;
	};
public:
	virtual HRESULT CreateGeometry(ID3D11Device* m_pDevice) = 0;
	virtual HRESULT CreateShaders(ID3D11Device* m_pDevice) = 0;
	virtual HRESULT CreateTextures(ID3D11Device* m_pDevice) = 0;
	virtual void Draw(const DirectX::XMMATRIX& vp,
						ID3D11DeviceContext* m_pDeviceContext) = 0;

	virtual HRESULT setRasterizerState(ID3D11Device* m_pDevice, D3D11_CULL_MODE cullMode);

	void Translate(DirectX::XMMATRIX translateMatrix);
	void Scale(DirectX::XMMATRIX scaleMatrix);
	void Rotate(DirectX::XMMATRIX rotateMatrix);

	void Clean();
protected:
	ID3D11Buffer* m_pIndexBuffer = NULL;
	ID3D11Buffer* m_pVertextBuffer = NULL;

	ID3D11RasterizerState* rasterizerState;

	VertexShader vs;
	PixelShader ps;
	ID3D11InputLayout* m_pInputLayout = NULL;

	std::vector<ID3D11Buffer*> constBuffers;
	std::vector<ID3D11SamplerState*> samplers;
	std::vector<ID3D11ShaderResourceView*> resources;

	DirectX::XMMATRIX model = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX translateMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX rotateMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 0.0f);

	SceneBuffer scBuffer;
};

class Cube : public Shape
{
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
	};

	struct Vertex
	{
		float x, y, z;
		float tx, ty, tz;
		float nx, ny, nz;
		float u, v;
	};
public:
	HRESULT CreateGeometry(ID3D11Device* m_pDevice) final;
	HRESULT CreateShaders(ID3D11Device* m_pDevice) final;
	HRESULT CreateTextures(ID3D11Device* m_pDevice) final;
	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext) final;
private:
	GeomBuffer geomBuffer;
};

class Square : public Shape
{
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
	};

	struct ColorBuffer
	{
		DirectX::XMVECTOR color;
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

	void setColor(DirectX::XMVECTOR color);

private:
	GeomBuffer geomBuffer;
	ColorBuffer colorBuffer;
};

class Sphere : public Shape
{
	struct GeomBuffer
	{
		DirectX::XMMATRIX modelMatrix;
	};

	struct ColorBuffer
	{
		DirectX::XMVECTOR color;
	};

	struct Vertex
	{
		float x, y, z;
		float u, v;
	};
public:
	HRESULT CreateGeometry(ID3D11Device* m_pDevice) final;
	HRESULT CreateShaders(ID3D11Device* m_pDevice) final;
	HRESULT CreateTextures(ID3D11Device* m_pDevice) final;
	void Draw(const DirectX::XMMATRIX& vp,
		ID3D11DeviceContext* m_pDeviceContext) final;

	void setColor(DirectX::XMVECTOR color);

private:
	GeomBuffer geomBuffer;
	ColorBuffer colorBuffer;
	int numIndeces;
};