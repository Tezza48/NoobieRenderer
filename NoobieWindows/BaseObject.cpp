#include "BaseObject.h"

BaseObject::BaseObject()
{
}

BaseObject::~BaseObject()
{
}

const DirectX::XMFLOAT3 & BaseObject::GetPosition() const
{
	return position;
}

void BaseObject::SetPosition(DirectX::XMFLOAT3 value)
{
	position = value;
}

const DirectX::XMFLOAT4 & BaseObject::GetRotation() const
{
	return rotation;
}

void BaseObject::SetRotation(DirectX::XMFLOAT4 value)
{
	rotation = value;
}

const float & BaseObject::GetScale() const
{
	return scale;
}

void BaseObject::SetScale(float value)
{
	scale = value;
}
