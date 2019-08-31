#pragma once
#include "NoobieCore\Camera.h"

class FlyCamera :
	public Camera
{
private:
	float angle;
	float pitch;

	float width;
	float height;

public:
	FlyCamera(float aspectRatio, float fov, float nearPlane, float farPlane);
	virtual ~FlyCamera();

	void Update(float dt) override;

	void Resize(int width, int height);
};

