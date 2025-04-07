#include "Shaders.h"

#include <fstream>
#include <vector>

void VertexShader::Clean()
{
	SAFE_RELEASE(shader);
	SAFE_RELEASE(shaderBuffer);
}

bool VertexShader::Initialize(ID3D11Device* device, std::wstring shaderpath, D3D_SHADER_MACRO* defines)
{
	ShaderInclude include;
	ID3DBlob* pErrMsg = nullptr;
	HRESULT hr = D3DCompileFromFile(shaderpath.c_str(), defines, &include, "main", "vs_5_0", NULL, NULL, &shaderBuffer, &pErrMsg);
	if (!SUCCEEDED(hr) && pErrMsg != nullptr)
	{
		OutputDebugStringA((const char*)pErrMsg->GetBufferPointer());
		return false;
	}

	hr = device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return shader;
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return shaderBuffer;
}

void PixelShader::Clean()
{
	SAFE_RELEASE(shader);
	SAFE_RELEASE(shaderBuffer);
}

bool PixelShader::Initialize(ID3D11Device* device, std::wstring shaderpath, D3D_SHADER_MACRO* defines)
{
	ShaderInclude include;
	ID3DBlob* pErrMsg = nullptr;
	HRESULT hr = D3DCompileFromFile(shaderpath.c_str(), defines, &include, "main", "ps_5_0", NULL, NULL, &shaderBuffer, &pErrMsg);
	if (!SUCCEEDED(hr) && pErrMsg != nullptr)
	{
		OutputDebugStringA((const char*)pErrMsg->GetBufferPointer());
		return false;
	}

	hr = device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &shader);
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return shader;
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return shaderBuffer;
}

HRESULT ShaderInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
{
	std::ifstream file(pFileName, std::ios::binary | std::ios::in);

	file.seekg(0, std::ios::end);
	size_t length = file.tellg();
	file.seekg(0, std::ios::beg);

	char* buffer = new char[length + 1];

	file.read(buffer, length);

	*ppData = buffer;
	*pBytes = (UINT)length;
	return S_OK;
}

HRESULT ShaderInclude::Close(LPCVOID pData)
{
	delete pData;
	return S_OK;
}
