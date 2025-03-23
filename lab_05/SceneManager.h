#pragma once

#include "framework.h"
#include <d3d11.h>

class SceneManager
{
    DirectX::XMMATRIX m_cameraXRotation;
    DirectX::XMMATRIX m_cameraYRotation;

    double m_animTime;
    bool m_isPlay;
    bool m_isTrapped;
    float m_zoomScene;
    POINT m_trapStart;
    POINT m_trapLast;

public:
    const float m_angelDel = 150.f;
    DirectX::XMMATRIX m_modelTransform;
    DirectX::XMMATRIX m_cameraTransform;

    SceneManager();
    void Update(double deltaTime);
    void OnLButtonDown(WPARAM wParam, LPARAM lParam);
    void OnLButtonUp(WPARAM wParam, LPARAM lParam);
    void OnMouseMove(WPARAM wParam, LPARAM lParam);
};