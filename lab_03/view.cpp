#include "view.h"

HRESULT View::Init() {
    m_pointOfInterest = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_distanceToPoint = 6.0f;
    m_phi = -XM_PIDIV4;
    m_theta = XM_PIDIV4;
    return S_OK;
}

void View::Frame() {
    XMFLOAT3 pos = XMFLOAT3(cosf(m_theta) * cosf(m_phi), sinf(m_theta), cosf(m_theta) * sinf(m_phi));
    pos.x = pos.x * m_distanceToPoint + m_pointOfInterest.x;
    pos.y = pos.y * m_distanceToPoint + m_pointOfInterest.y;
    pos.z = pos.z * m_distanceToPoint + m_pointOfInterest.z;
    float upTheta = m_theta + XM_PIDIV2;
    XMFLOAT3 up = XMFLOAT3(cosf(upTheta) * cosf(m_phi), sinf(upTheta), cosf(upTheta) * sinf(m_phi));

    m_viewMatrix = DirectX::XMMatrixLookAtLH(
        DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f),
        DirectX::XMVectorSet(m_pointOfInterest.x, m_pointOfInterest.y, m_pointOfInterest.z, 0.0f),
        DirectX::XMVectorSet(up.x, up.y, up.z, 0.0f)
    );
}


void View::MouseMoved(float dx, float dy, float wheel) {
    m_phi += dx / 100.0f;
    m_theta += dy / 100.0f;
    m_theta = min(max(m_theta, -XM_PIDIV2), XM_PIDIV2);
    m_distanceToPoint -= wheel / 100.0f;
    if (m_distanceToPoint < 1.0f) {
        m_distanceToPoint = 1.0f;
    }
}