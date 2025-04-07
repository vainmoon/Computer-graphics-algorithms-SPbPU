#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	posVector = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	rotVector = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	fov = fovRadians;
	n = nearZ;
	projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, farZ, nearZ);
}

const DirectX::XMMATRIX& Camera::GetViewMatrix() const
{
	return viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	return posVector;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	return rotVector;
}

void Camera::SetPosition(const XMVECTOR& pos)
{
	posVector = pos;
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos)
{
	posVector += pos;
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	rotVector = rot;
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rot)
{
	rotVector += rot;
	UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, posVector);
	if (pos.x == lookAtPos.x && pos.y == lookAtPos.y && pos.z == lookAtPos.z)
		return;

	lookAtPos.x = pos.x - lookAtPos.x;
	lookAtPos.y = pos.y - lookAtPos.y;
	lookAtPos.z = pos.z - lookAtPos.z;

	double pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const double distance = sqrt((double)lookAtPos.x * lookAtPos.x + (double)lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	double yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	SetRotation(XMVectorSet(static_cast<float>(pitch), static_cast<float>(yaw), 0.0f, 1.0f));
}

const XMVECTOR& Camera::GetForwardVector()
{
	return vec_forward;
}

const XMVECTOR& Camera::GetRightVector()
{
	return vec_right;
}

const XMVECTOR& Camera::GetBackwardVector()
{
	return vec_backward;
}

const XMVECTOR& Camera::GetLeftVector()
{
	return vec_left;
}

float Camera::GetFov()
{
	return fov;
}

float Camera::GetNearPlane()
{
	return n;
}

void Camera::UpdateViewMatrix()
{
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotVector);
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	camTarget += posVector;
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
	viewMatrix = XMMatrixLookAtLH(posVector, camTarget, upDir);
	
	XMFLOAT3 rot;
	XMStoreFloat3(&rot, rotVector);
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}
