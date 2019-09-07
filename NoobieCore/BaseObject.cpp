#include "BaseObject.h"

using namespace DirectX;

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

void BaseObject::SetPosition(XMFLOAT3 value)
{
	position = value;
}

const DirectX::XMFLOAT4 & BaseObject::GetRotation() const
{
	return rotation;
}

void BaseObject::SetRotation(XMFLOAT4 value)
{
	auto normalizedS = XMQuaternionNormalize(XMLoadFloat4(&value));
	XMStoreFloat4(&rotation, normalizedS);
}

const float & BaseObject::GetScale() const
{
	return scale;
}

void BaseObject::SetScale(float value)
{
	scale = value;
}

void BaseObject::SetDoUpdate(bool value)
{
	doUpdate = value;
}

bool BaseObject::GetDoUpdate() const
{
	return doUpdate;
}
