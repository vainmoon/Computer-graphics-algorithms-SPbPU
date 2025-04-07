#include "Light.h"

#include <string>

#define SAFE_RELEASE(p) if (p != NULL) { p->Release(); p = NULL; }

HRESULT Light::createBuffer(ID3D11Device* device)
{
    D3D11_BUFFER_DESC desc;
    desc = { 0 };
    desc.ByteWidth = sizeof(LightBuffer);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    HRESULT hr = device->CreateBuffer(&desc, NULL, &lightBuffer);

    if (SUCCEEDED(hr))
    {
        desc = { 0 };
        desc.ByteWidth = sizeof(SceneBuffer);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        hr = device->CreateBuffer(&desc, NULL, &sceneBuffer);
    }
    
    return hr;
}

void Light::updateBuffer(ID3D11DeviceContext* m_pDeviceContext)
{
    m_pDeviceContext->UpdateSubresource(lightBuffer, 0, nullptr, &light, 0, 0);
    m_pDeviceContext->UpdateSubresource(sceneBuffer, 0, nullptr, &scBuffer, 0, 0);
    m_pDeviceContext->PSSetConstantBuffers(1, 1, &lightBuffer);
    m_pDeviceContext->PSSetConstantBuffers(0, 1, &sceneBuffer);
}

void Light::setAmbient(float r, float g, float b, float intensity)
{
    light.ambientColor[0] = r;
    light.ambientColor[1] = g;
    light.ambientColor[2] = b;
    light.ambientColor[3] = intensity;
}

void Light::RenderImGUI(ID3D11Device* device, ID3D11DeviceContext* m_pDeviceContext)
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Light");

    ImGui::DragFloat4("Ambient", light.ambientColor, 0.01f);

    if (ImGui::Button("+") && lightsShapes.size() < 10)
    {
        addLight(device, m_pDeviceContext);
    }

    ImGui::DragFloat("Diffuse coefficient", &light.diffuseCoef, 0.01f);
    ImGui::DragFloat("Specular coefficient", &light.specularCoef, 0.01f);
    ImGui::DragFloat("Shine coefficient", &light.shine, 0.01f);
    for (int i = 0; i < lightsShapes.size(); i++)
    {
        std::string title = "Light " + std::to_string(i);
        ImGui::Text(title.c_str());
        std::string posTitle = "Pos" + std::to_string(i);
        ImGui::DragFloat3(posTitle.c_str(), light.lightsDesc[i].pos, 0.01f);
        std::string colorTitle = "Color" + std::to_string(i);
        ImGui::DragFloat3(colorTitle.c_str(), light.lightsDesc[i].color, 0.01f);
    }

    ImGui::End();

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Light::addLight(ID3D11Device* device, ID3D11DeviceContext* m_pDeviceContext)
{
    Sphere sphere;

    sphere.setRasterizerState(device, D3D11_CULL_MODE::D3D11_CULL_BACK);
    sphere.CreateShaders(device);
    sphere.CreateGeometry(device);
    sphere.setColor(DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
    sphere.Scale(DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f));

    PointLight pointLight;
    pointLight.pos[0] = 0.0f;
    pointLight.pos[1] = 2.0f;
    pointLight.pos[2] = 0.0f;
    pointLight.pos[3] = 1.0f;

    pointLight.color[0] = 1.0f;
    pointLight.color[1] = 0.0f;
    pointLight.color[2] = 0.0f;
    pointLight.color[3] = 1.0f;

    light.lightsDesc[lightsShapes.size()] = pointLight;

    lightsShapes.push_back(sphere);

    light.lightsCount = static_cast<int>(lightsShapes.size());
    light.diffuseCoef = 0.5f;
    light.shine = 20.0f;
    light.specularCoef = 0.9f;
}

void Light::Draw(const DirectX::XMMATRIX& vp, ID3D11DeviceContext* m_pDeviceContext)
{
    for (int i = 0; i < lightsShapes.size(); i++)
    {
        lightsShapes[i].Translate(DirectX::XMMatrixTranslation(light.lightsDesc[i].pos[0], light.lightsDesc[i].pos[1], light.lightsDesc[i].pos[2]));
        lightsShapes[i].setColor(DirectX::XMVectorSet(light.lightsDesc[i].color[0], light.lightsDesc[i].color[1], light.lightsDesc[i].color[2], 1.0f));
        lightsShapes[i].Draw(vp, m_pDeviceContext);
    }
}

void Light::setCamPos(DirectX::XMVECTOR camPos)
{
    scBuffer.cameraPos = camPos;
}

void Light::Clean()
{
    SAFE_RELEASE(lightBuffer);
    SAFE_RELEASE(sceneBuffer);
    for (auto& lightShape : lightsShapes)
    {
        lightShape.Clean();
    }
}
