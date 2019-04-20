#include "PlayerSpaceship.h"
#include "Assets.h"

PlayerSpaceship::PlayerSpaceship(ID3D11Device * device, Camera * camera):
	Renderable(device, Assets::LoadObj(Assets::ModelEnum::COCKPIT_OBJ, 1.0f))
{
	cameraRef = camera;
	doUpdate = true;
}

PlayerSpaceship::~PlayerSpaceship()
{
}

void PlayerSpaceship::Update(float dt, const Input & input)
{
	auto currentRotation = XMLoadFloat4(&rotation);
	auto right = XMQuaternionMultiply(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), currentRotation);
	auto forward = XMQuaternionMultiply(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), currentRotation);

	auto newPosition = XMLoadFloat3(&position);

	XMVECTOR moveDelta;

	XMFLOAT2 moveInput = {};

	if (input.GetKey(Input::KB_W))
	{
		moveInput.y++;
	}
	if (input.GetKey(Input::KB_S))
	{
		moveInput.y--;
	}
	if (input.GetKey(Input::KB_A))
	{
		moveInput.x--;
	}
	if (input.GetKey(Input::KB_D))
	{
		moveInput.x++;
	}
	//if (input.GetKey(Input::KB_R))
	//{
	//	position.y += dt;
	//}
	//if (input.GetKey(Input::KB_F))
	//{
	//	position.y -= dt;
	//}

	moveDelta = forward * moveInput.y + right * moveInput.x;
	moveDelta = XMVector3Normalize(moveDelta);

	newPosition += moveDelta * dt;

	XMStoreFloat3(&position, newPosition);

	float sense = 1.0f;

	auto mouse = input.GetMouseDelta();
	auto newRot = XMLoadFloat4(&rotation);

	newRot = XMQuaternionMultiply(newRot, XMQuaternionRotationAxis(right, mouse.y * sense));
	newRot = XMQuaternionMultiply(newRot, XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), mouse.x * sense));
	
	XMStoreFloat4(&rotation, newRot);

	printf("\rMouse x: %f, y: %f", mouse.x, mouse.y);

	// Update the camera's position
	cameraRef->SetPosition(position);
	cameraRef->SetRotation(rotation);
}
