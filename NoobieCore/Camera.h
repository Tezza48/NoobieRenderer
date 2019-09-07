#pragma once
#include "Renderable.h"
#include <DirectXMath.h>
#include "Input.h"

class Camera :
	public Renderable
{
private:
	float aspectRatio;

	float fov;

	float nearPlane;
	float farPlane;

	DirectX::XMFLOAT4X4 proj;

	bool isProjDirty;

private:
	Camera() = delete;

public:
	Camera(float aspectRatio, float fov, float nearPlane, float farPlane);

	void SetIsVisible() = delete;
	void SetDoUpdate() = delete;

	DirectX::XMFLOAT4X4 GetView() const;
	DirectX::XMFLOAT4X4 GetProj() const;

	virtual void Update(float dt) override;

	void LookAt(XMFLOAT3 position);

	void SetAspectRatio(float value);
};
