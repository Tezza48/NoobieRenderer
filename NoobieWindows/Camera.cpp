#include "Camera.h"

using namespace DirectX;

Camera::Camera(float aspectRatio, float fov, float nearPlane, float farPlane) :
	aspectRatio(aspectRatio), fov(fov), nearPlane(nearPlane), farPlane(farPlane)
{
	isVisible = false;
	doUpdate = true;
	isProjDirty = true;
}

Camera::~Camera()
{
}

XMMATRIX Camera::GetView() const
{
	auto world = GetWorld();
	auto det = XMMatrixDeterminant(world);

	return XMMatrixInverse(&det, world);
}

XMMATRIX Camera::GetProj() const
{
	return proj;
}

void Camera::Update(float dt, const Input & input)
{
	if (isProjDirty)
	{
		proj = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
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
