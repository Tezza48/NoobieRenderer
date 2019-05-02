#pragma once
#include "Renderable.h"
#include <DirectXMath.h>

class Camera :
	public Renderable
{
private:
	float aspectRatio;

	float fov;

	float nearPlane;
	float farPlane;

	DirectX::XMMATRIX proj;

	bool isProjDirty;

private:
	Camera() = delete;

public:
	Camera(float aspectRatio, float fov, float nearPlane, float farPlane);
	~Camera();

	void SetIsVisible() = delete;
	void SetDoUpdate() = delete;

	DirectX::XMMATRIX GetView() const;
	DirectX::XMMATRIX GetProj() const;

	void Update(float dt, const Input & input) override;

	void LookAt(XMFLOAT3 position);

	void SetAspectRatio(float value);
};

