#include "Shapes.h"
#include "DDSTextureLoader11.h"

#include <vector>
#include <math.h>

HRESULT Cube::CreateGeometry(ID3D11Device* m_pDevice)
{
    static const Vertex v[] = {
        {-0.5, -0.5,  0.5, 1, 0, 0,  0.0f,  -1.0f, 0.0f, 0, 1},
        { 0.5, -0.5,  0.5, 1, 0, 0, 0.0f,  -1.0f, 0.0f, 1, 1},
        { 0.5, -0.5, -0.5, 1, 0, 0, 0.0f,  -1.0f, 0.0f, 1, 0},
        {-0.5, -0.5, -0.5, 1, 0, 0, 0.0f,  -1.0f, 0.0f, 0, 0},

        {-0.5,  0.5, -0.5, 1, 0, 0, 0.0f, 1.0f, 0.0f, 0, 1},
        { 0.5,  0.5, -0.5, 1, 0, 0, 0.0f, 1.0f, 0.0f, 1, 1},
        { 0.5,  0.5,  0.5, 1, 0, 0, 0.0f, 1.0f, 0.0f, 1, 0},
        {-0.5,  0.5,  0.5, 1, 0, 0, 0.0f, 1.0f, 0.0f, 0, 0},

        { 0.5, -0.5, -0.5, 0, 0, 1, 1.0f,  0.0f,  0.0f, 0, 1},
        { 0.5, -0.5,  0.5, 0, 0, 1, 1.0f,  0.0f,  0.0f, 1, 1},
        { 0.5,  0.5,  0.5, 0, 0, 1, 1.0f,  0.0f,  0.0f, 1, 0},
        { 0.5,  0.5, -0.5, 0, 0, 1, 1.0f,  0.0f,  0.0f, 0, 0},

        {-0.5, -0.5,  0.5, 0, 0, -1, -1.0f,  0.0f,  0.0f, 0, 1},
        {-0.5, -0.5, -0.5, 0, 0, -1, -1.0f,  0.0f,  0.0f, 1, 1},
        {-0.5,  0.5, -0.5, 0, 0, -1, -1.0f,  0.0f,  0.0f, 1, 0},
        {-0.5,  0.5,  0.5, 0, 0, -1, -1.0f,  0.0f,  0.0f, 0, 0},

        { 0.5, -0.5,  0.5, -1, 0, 0, 0.0f, 0.0f, 1.0f, 0, 1},
        {-0.5, -0.5,  0.5, -1, 0, 0, 0.0f, 0.0f, 1.0f, 1, 1},
        {-0.5,  0.5,  0.5, -1, 0, 0, 0.0f, 0.0f, 1.0f, 1, 0},
        { 0.5,  0.5,  0.5, -1, 0, 0, 0.0f, 0.0f, 1.0f, 0, 0},

        {-0.5, -0.5, -0.5, 1, 0, 0, 0.0f, 0.0f, -1.0f, 0, 1},
        { 0.5, -0.5, -0.5, 1, 0, 0, 0.0f, 0.0f, -1.0f, 1, 1},
        { 0.5,  0.5, -0.5, 1, 0, 0, 0.0f, 0.0f, -1.0f, 1, 0},
        {-0.5,  0.5, -0.5, 1, 0, 0, 0.0f, 0.0f, -1.0f, 0, 0},
    };

    static const DWORD Indices[] = {
        0, 2, 1, 0, 3, 2,
        4, 6, 5, 4, 7, 6,
        8, 10, 9, 8, 11, 10,
        12, 14, 13, 12, 15, 14,
        16, 18, 17, 16, 19, 18,
        20, 22, 21, 20, 23, 22,
    };

    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(data));
    data.pSysMem = v;

    HRESULT hr = m_pDevice->CreateBuffer(&desc, &data, &m_pVertextBuffer);

    if (SUCCEEDED(hr))
    {
        desc = {};
        desc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(Indices);
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        data.pSysMem = &Indices;
        data.SysMemPitch = sizeof(Indices);
        data.SysMemSlicePitch = 0;

        hr = m_pDevice->CreateBuffer(&desc, &data, &m_pIndexBuffer);
    }

    ID3D11Buffer* m_pSceneBuffer;

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(SceneBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = m_pDevice->CreateBuffer(&desc, NULL, &m_pSceneBuffer);
    }

    if (SUCCEEDED(hr))
    {
        constBuffers.push_back(m_pSceneBuffer);
    }

    ID3D11Buffer* m_pGeomBuffer;

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(GeomBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = m_pDevice->CreateBuffer(&desc, NULL, &m_pGeomBuffer);
    }

    if (SUCCEEDED(hr))
    {
        constBuffers.push_back(m_pGeomBuffer);
    }

    return hr;
}

HRESULT Cube::CreateShaders(ID3D11Device* m_pDevice)
{
    static const D3D11_INPUT_ELEMENT_DESC InputDesc[] = { 
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    D3D_SHADER_MACRO defines[] = { "USE_TEXTURE", "1", "APPLY_LIGHT", "1", "USE_NORMAL_MAP", "1", NULL, NULL};
    if (!vs.Initialize(m_pDevice, L"ShapeVS.hlsl", defines))
    {
        return S_FALSE;
    }

    HRESULT result = m_pDevice->CreateInputLayout(InputDesc, ARRAYSIZE(InputDesc), vs.GetBuffer()->GetBufferPointer(), vs.GetBuffer()->GetBufferSize(), &m_pInputLayout);
    
    if (!ps.Initialize(m_pDevice, L"ShapePS.hlsl", defines))
    {
        return S_FALSE;
    }

    return result;
}

HRESULT Cube::CreateTextures(ID3D11Device* m_pDevice)
{
    ID3D11SamplerState* m_pSampler;
    ID3D11ShaderResourceView* m_pTextureView;

    HRESULT result = DirectX::CreateDDSTextureFromFile(m_pDevice, L"textureWithNormalMap/breaks.dds", nullptr, &m_pTextureView);

    if (SUCCEEDED(result))
    {
        resources.push_back(m_pTextureView);

        D3D11_SAMPLER_DESC desc = {};
        desc.Filter = D3D11_FILTER_ANISOTROPIC;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MinLOD = -FLT_MAX;
        desc.MaxLOD = FLT_MAX;
        desc.MipLODBias = 0.0f;
        desc.MaxAnisotropy = 16;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 1.0f;
        result = m_pDevice->CreateSamplerState(&desc, &m_pSampler);
    }

    ID3D11ShaderResourceView* m_pTextureView1;

    result = DirectX::CreateDDSTextureFromFile(m_pDevice, L"textureWithNormalMap/breaks_norm.dds", nullptr, &m_pTextureView1);

    if (SUCCEEDED(result))
    {
        resources.push_back(m_pTextureView1);
    }

    if (SUCCEEDED(result))
    {
        samplers.push_back(m_pSampler);
    }

    return result;
}

void Cube::Draw(const DirectX::XMMATRIX& vp, ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->RSSetState(rasterizerState);

    model = DirectX::XMMatrixIdentity();
    model = rotateMatrix * scaleMatrix * translateMatrix;
    geomBuffer.modelMatrix = model;
    geomBuffer.modelMatrix = DirectX::XMMatrixTranspose(geomBuffer.modelMatrix);
    scBuffer.vp = vp;
    scBuffer.vp = DirectX::XMMatrixTranspose(scBuffer.vp);
    m_pDeviceContext->UpdateSubresource(constBuffers[0], 0, nullptr, &scBuffer, 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[1], 0, nullptr, &geomBuffer, 0, 0);

    m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    m_pDeviceContext->VSSetShader(vs.GetShader(), NULL, 0);
    m_pDeviceContext->PSSetShader(ps.GetShader(), NULL, 0);

    m_pDeviceContext->VSSetConstantBuffers(0, 2, constBuffers.data());

    if (!samplers.empty() && !resources.empty())
    {
        m_pDeviceContext->PSSetSamplers(0, 1, samplers.data());

        m_pDeviceContext->PSSetShaderResources(0, 2, resources.data());
    }
    
    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);

    m_pDeviceContext->DrawIndexed(36, 0, 0);
}

HRESULT Shape::setRasterizerState(ID3D11Device* m_pDevice, D3D11_CULL_MODE cullMode)
{
    HRESULT result;
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = cullMode;
    result = m_pDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

    return result;
}

void Shape::Translate(DirectX::XMMATRIX translateMatrix)
{
    this->translateMatrix = translateMatrix;
}

void Shape::Scale(DirectX::XMMATRIX scaleMatrix)
{
    this->scaleMatrix = scaleMatrix;
}

void Shape::Rotate(DirectX::XMMATRIX rotateMatrix)
{
    this->rotateMatrix = rotateMatrix;
}

void Shape::Clean()
{
    vs.Clean();
    ps.Clean();
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pVertextBuffer);
    for (auto& buf : constBuffers)
    {
        SAFE_RELEASE(buf);
    }
    for (auto& res : resources)
    {
        SAFE_RELEASE(res);
    }
    for (auto& sampler : samplers)
    {
        SAFE_RELEASE(sampler);
    }
    SAFE_RELEASE(m_pInputLayout);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(rasterizerState);
}

HRESULT Square::CreateGeometry(ID3D11Device* m_pDevice)
{
    static const Vertex v[] = {
        {-0.5, -0.5, 0.5},
        {0.5, -0.5, 0.5},
        {0.5, -0.5, -0.5},
        {-0.5, -0.5, -0.5},
    };

    static const DWORD Indices[] = {
        0, 2, 1, 0, 3, 2,
    };

    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));

    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(data));
    data.pSysMem = v;

    HRESULT hr = m_pDevice->CreateBuffer(&desc, &data, &m_pVertextBuffer);

    if (SUCCEEDED(hr))
    {
        desc = {};
        desc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(Indices);
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        data.pSysMem = &Indices;
        data.SysMemPitch = sizeof(Indices);
        data.SysMemSlicePitch = 0;

        hr = m_pDevice->CreateBuffer(&desc, &data, &m_pIndexBuffer);
    }

    ID3D11Buffer* m_pSceneBuffer;

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(SceneBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = m_pDevice->CreateBuffer(&desc, NULL, &m_pSceneBuffer);
    }

    if (SUCCEEDED(hr))
    {
        constBuffers.push_back(m_pSceneBuffer);
    }

    ID3D11Buffer* m_pGeomBuffer;

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(GeomBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = m_pDevice->CreateBuffer(&desc, NULL, &m_pGeomBuffer);
    }

    if (SUCCEEDED(hr))
    {
        constBuffers.push_back(m_pGeomBuffer);
    }

    ID3D11Buffer* m_pColorBuffer;

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(ColorBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = m_pDevice->CreateBuffer(&desc, NULL, &m_pColorBuffer);
    }

    if (SUCCEEDED(hr))
    {
        constBuffers.push_back(m_pColorBuffer);
    }

    return hr;
}

HRESULT Square::CreateShaders(ID3D11Device* m_pDevice)
{
    static const D3D11_INPUT_ELEMENT_DESC InputDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    if (!vs.Initialize(m_pDevice, L"ShapeVS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    HRESULT result = m_pDevice->CreateInputLayout(InputDesc, ARRAYSIZE(InputDesc), vs.GetBuffer()->GetBufferPointer(), vs.GetBuffer()->GetBufferSize(), &m_pInputLayout);

    if (!ps.Initialize(m_pDevice, L"ShapePS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    return result;
}

HRESULT Square::CreateTextures(ID3D11Device* m_pDevice)
{
    return E_NOTIMPL;
}

void Square::Draw(const DirectX::XMMATRIX& vp, ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->RSSetState(rasterizerState);

    model = DirectX::XMMatrixIdentity();
    model = rotateMatrix * scaleMatrix * translateMatrix;
    geomBuffer.modelMatrix = model;
    geomBuffer.modelMatrix = DirectX::XMMatrixTranspose(geomBuffer.modelMatrix);
    scBuffer.vp = vp;
    scBuffer.vp = DirectX::XMMatrixTranspose(scBuffer.vp);
    m_pDeviceContext->UpdateSubresource(constBuffers[0], 0, nullptr, &scBuffer, 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[1], 0, nullptr, &geomBuffer, 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[2], 0, nullptr, &colorBuffer, 0, 0);

    m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    m_pDeviceContext->VSSetShader(vs.GetShader(), NULL, 0);
    m_pDeviceContext->PSSetShader(ps.GetShader(), NULL, 0);

    m_pDeviceContext->VSSetConstantBuffers(0, 2, constBuffers.data());
    m_pDeviceContext->PSSetConstantBuffers(2, 1, &constBuffers[2]);

    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);

    m_pDeviceContext->DrawIndexed(6, 0, 0);
}

void Square::setColor(DirectX::XMVECTOR color)
{
    colorBuffer.color = color;
}

HRESULT Sphere::CreateGeometry(ID3D11Device* m_pDevice)
{
    static std::vector<Vertex> Vertices;

    static std::vector<DWORD> Indices;

    int stackCount = 20;
    int sliceCount = 20;
    float phiStep = DirectX::XM_PI / stackCount;
    float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;
    float radius = 0.5f;
    Vertices.push_back({ 0, radius, 0 });

    for (int i = 1; i < stackCount; i++)
    {
        double phi = static_cast<double>(i * phiStep);
        for (int j = 0; j <= sliceCount; j++)
        {
            double theta = static_cast<double>(j * thetaStep);
            Vertex v = { static_cast<float>(radius * sin(phi) * cos(theta)),
                                static_cast<float>(radius * cos(phi)),
                                static_cast<float>(radius * sin(phi) * sin(theta) )};
            Vertices.push_back(v);
        }
    }
    Vertices.push_back({ 0, -radius, 0 });

    for (int i = 1; i <= sliceCount; i++) {
        Indices.push_back(0);
        Indices.push_back(i + 1);
        Indices.push_back(i);
    }

    int baseIndex = 1;
    int ringVertexCount = sliceCount + 1;
    for (int i = 0; i < stackCount - 2; i++) {
        for (int j = 0; j < sliceCount; j++) {
            Indices.push_back(baseIndex + i * ringVertexCount + j);
            Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    int southPoleIndex = static_cast<int>(Vertices.size() - 1);
    baseIndex = southPoleIndex - ringVertexCount;
    for (int i = 0; i < sliceCount; i++) {
        Indices.push_back(southPoleIndex);
        Indices.push_back(baseIndex + i);
        Indices.push_back(baseIndex + i + 1);
    }

    D3D11_SUBRESOURCE_DATA data;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = static_cast<UINT>(Vertices.size() * sizeof(Vertex));
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    data.pSysMem = Vertices.data();
    data.SysMemPitch = static_cast<UINT>(Vertices.size() * sizeof(Vertex));
    data.SysMemSlicePitch = 0;

    HRESULT result = m_pDevice->CreateBuffer(&desc, &data, &m_pVertextBuffer);

    if (SUCCEEDED(result))
    {
        numIndeces = (int)Indices.size();
        desc = {};
        desc.ByteWidth = static_cast<UINT>(Indices.size() * sizeof(DWORD));
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        data.pSysMem = Indices.data();
        data.SysMemPitch = static_cast<UINT>(Indices.size() * sizeof(DWORD));
        data.SysMemSlicePitch = 0;

        result = m_pDevice->CreateBuffer(&desc, &data, &m_pIndexBuffer);
    }

    ID3D11Buffer* m_pSceneBuffer;

    if (SUCCEEDED(result))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(SceneBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        result = m_pDevice->CreateBuffer(&desc, NULL, &m_pSceneBuffer);
    }

    if (SUCCEEDED(result))
    {
        constBuffers.push_back(m_pSceneBuffer);
    }

    ID3D11Buffer* m_pGeomBuffer;

    if (SUCCEEDED(result))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(GeomBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        result = m_pDevice->CreateBuffer(&desc, NULL, &m_pGeomBuffer);
    }

    if (SUCCEEDED(result))
    {
        constBuffers.push_back(m_pGeomBuffer);
    }

    ID3D11Buffer* m_pColorBuffer;

    if (SUCCEEDED(result))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(ColorBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        result = m_pDevice->CreateBuffer(&desc, NULL, &m_pColorBuffer);
    }

    if (SUCCEEDED(result))
    {
        constBuffers.push_back(m_pColorBuffer);
    }

    return result;
}

HRESULT Sphere::CreateShaders(ID3D11Device* m_pDevice)
{
    static const D3D11_INPUT_ELEMENT_DESC InputDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    if (!vs.Initialize(m_pDevice, L"ShapeVS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    HRESULT result = m_pDevice->CreateInputLayout(InputDesc, ARRAYSIZE(InputDesc), vs.GetBuffer()->GetBufferPointer(), vs.GetBuffer()->GetBufferSize(), &m_pInputLayout);
    if (!ps.Initialize(m_pDevice, L"ShapePS.hlsl", nullptr))
    {
        return S_FALSE;
    }

    return result;
}

HRESULT Sphere::CreateTextures(ID3D11Device* m_pDevice)
{
    return S_OK;
}

void Sphere::Draw(const DirectX::XMMATRIX& vp,
    ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->RSSetState(rasterizerState);

    model = DirectX::XMMatrixIdentity();
    model = scaleMatrix * translateMatrix;
    geomBuffer.modelMatrix = model;
    geomBuffer.modelMatrix = DirectX::XMMatrixTranspose(geomBuffer.modelMatrix);
    scBuffer.vp = vp;
    scBuffer.vp = DirectX::XMMatrixTranspose(scBuffer.vp);
    m_pDeviceContext->UpdateSubresource(constBuffers[0], 0, nullptr, &scBuffer, 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[1], 0, nullptr, &geomBuffer, 0, 0);
    m_pDeviceContext->UpdateSubresource(constBuffers[2], 0, nullptr, &colorBuffer, 0, 0);

    m_pDeviceContext->IASetInputLayout(m_pInputLayout);

    m_pDeviceContext->VSSetShader(vs.GetShader(), NULL, 0);
    m_pDeviceContext->PSSetShader(ps.GetShader(), NULL, 0);

    m_pDeviceContext->VSSetConstantBuffers(0, 2, constBuffers.data());
    m_pDeviceContext->PSSetConstantBuffers(2, 1, &constBuffers[2]);

    if (!samplers.empty() && !resources.empty())
    {
        m_pDeviceContext->PSSetSamplers(0, 1, samplers.data());

        m_pDeviceContext->PSSetShaderResources(0, 1, resources.data());
    }

    m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertextBuffer, &stride, &offset);

    m_pDeviceContext->DrawIndexed(numIndeces, 0, 0);
}

void Sphere::setColor(DirectX::XMVECTOR color)
{
    colorBuffer.color = color;
}
