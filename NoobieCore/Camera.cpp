#include "Camera.h"

using namespace DirectX;

Camera::Camera(float aspectRatio, float fov, float nearPlane, float farPlane) : 
	Renderable(), aspectRatio(aspectRatio), fov(fov), nearPlane(nearPlane), farPlane(farPlane)
{
	proj = {};

	isVisible = false;
	doUpdate = true;
	isProjDirty = true;
}

XMFLOAT4X4 Camera::GetView() const
{
	auto worldS = XMLoadFloat4x4(&GetWorld());
	auto det = XMMatrixDeterminant(worldS);

	auto invS = XMMatrixInverse(&det, worldS);

	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, invS);

	return view;
}

XMFLOAT4X4 Camera::GetProj() const
{
	return proj;
}

void Camera::Update(float dt)
{
	if (isProjDirty)
	{
		XMStoreFloat4x4(&proj, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));
	}
}

void Camera::LookAt(XMFLOAT3 targetPosition)
{
	auto lookAt = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&targetPosition), XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));

	auto det = XMMatrixDeterminant(lookAt);
	lookAt = XMMatrixInverse(&det, lookAt);

	XMVECTOR scale;
	XMVECTOR pos;
	XMVECTOR rot;

	XMMatrixDecompose(&scale, &rot, &pos, lookAt);

	//XMStoreFloat3(&position, pos);
	XMStoreFloat4(&rotation, rot);
}

void Camera::SetAspectRatio(float value)
{
	aspectRatio = value;
	isProjDirty = true;
}
