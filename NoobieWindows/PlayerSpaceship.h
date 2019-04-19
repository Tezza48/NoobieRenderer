#pragma once
#include "Renderable.h"
#include "Camera.h"

class PlayerSpaceship :
	public Renderable
{
private:
	Camera * cameraRef;
	float angle;

public:
	PlayerSpaceship(ID3D11Device * device, Camera * camera);
	~PlayerSpaceship();

	void Update(float dt, const Input & input) override;
};

