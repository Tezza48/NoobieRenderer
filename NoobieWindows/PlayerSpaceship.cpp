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
	if (input.GetKey(Input::KB_W))
	{
		position.z += dt;
	}
	if (input.GetKey(Input::KB_S))
	{
		position.z -= dt;
	}
	if (input.GetKey(Input::KB_A))
	{
		position.x -= dt;
	}
	if (input.GetKey(Input::KB_D))
	{
		position.x += dt;
	}
	if (input.GetKey(Input::KB_R))
	{
		position.y += dt;
	}
	if (input.GetKey(Input::KB_F))
	{
		position.y -= dt;
	}

	float sense = 10.0f;

	auto mouse = input.GetMousePosition();
	//auto newRot = XMQuaternionRotationRollPitchYaw(mouse.y * sense, mouse.x * sense, 0.0f);XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f)

	//auto up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	//auto right = XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f);

	//auto pitch = XMQuaternionRotationAxis(right, mouse.y * sense);
	//auto yaw = XMQuaternionRotationAxis(up, mouse.x * sense);

	//auto newRot = XMLoadFloat4(&rotation) * (pitch + yaw);

	//XMStoreFloat4(&rotation, newRot);

	printf("\rMouse x: %f, y: %f", mouse.x, mouse.y);

	//position = { sin(angle) * 4.0f, 8.0f, cos(angle) * 4.0f };
	//XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYaw(0.0f, angle, 0.0f));

	// Update the camera's position
	cameraRef->SetPosition(position);
	cameraRef->SetRotation(rotation);
}
