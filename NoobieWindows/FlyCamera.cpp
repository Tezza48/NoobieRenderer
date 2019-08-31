#include "FlyCamera.h"



FlyCamera::FlyCamera(float aspectRatio, float fov, float nearPlane, float farPlane) :
	Camera(aspectRatio, fov, nearPlane, farPlane), angle(0), pitch(0), width(0), height(0)
{

}


FlyCamera::~FlyCamera()
{
}

void FlyCamera::Update(float dt)
{
	Camera::Update(dt);

	float speed = 2.5 * dt;

	XMFLOAT3 posInput(0, 0, 0);
	if (Input::GetKey(GLFW_KEY_A))
	{
		posInput.x -= speed;
	}

	if (Input::GetKey(GLFW_KEY_D))
	{
		posInput.x += speed;
	}

	if (Input::GetKey(GLFW_KEY_S))
	{
		posInput.z -= speed;
	}

	if (Input::GetKey(GLFW_KEY_W))
	{
		posInput.z += speed;
	}

	if (Input::GetKey(GLFW_KEY_Q))
	{
		posInput.y -= speed;
	}

	if (Input::GetKey(GLFW_KEY_E))
	{
		posInput.y += speed;
	}

	angle += (Input::GetDeltaMouse().x / width) * dt * 100.0f;
	pitch += (Input::GetDeltaMouse().y / height) * dt * 100.0f;

	pitch = min(max(pitch, -3.1f / 2.0f), 3.1f / 2.0f);


	XMFLOAT4 rotFloat4;
	auto newRot = XMQuaternionRotationRollPitchYaw(pitch, angle, 0.0f);
	XMStoreFloat4(&rotFloat4, newRot);

	SetRotation(rotFloat4);

	XMVECTOR right = XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), newRot) * posInput.x;
	XMVECTOR up = XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), newRot) * posInput.y;
	XMVECTOR fwd = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), newRot) * posInput.z;

	auto delta = fwd + right + up;

	auto lastPos = XMLoadFloat3(&position);

	auto newPos = lastPos + delta;

	XMFLOAT3 newPosf3;
	XMStoreFloat3(&newPosf3, newPos);

	position = newPosf3;
}

void FlyCamera::Resize(int width, int height)
{
	this->width = static_cast<float>(width);
	this->height = static_cast<float>(height);
}
