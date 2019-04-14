#pragma once
#include <DirectXMath.h>
#include "Input.h"

class BaseObject
{
private:

protected:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 rotation;
	float scale = 1.0f;
public:

	BaseObject();
	virtual ~BaseObject();

	virtual void Update(float dt, const Input & input) = 0;

	const DirectX::XMFLOAT3 & GetPosition() const;
	void SetPosition(DirectX::XMFLOAT3 value);

	const DirectX::XMFLOAT4 & GetRotation() const;
	void SetRotation(DirectX::XMFLOAT4 value);

	const float & GetScale() const;
	void SetScale(float value);
};

