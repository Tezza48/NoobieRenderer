#include "Renderable.h"

using namespace DirectX;

Renderable::Renderable(ID3D11Device * device, MeshData mesh) : BaseObject()
{
	position = {};

	XMStoreFloat4(&rotation, XMQuaternionIdentity());
	vb.Init(device, mesh.vertices);
	ib.Init(device, mesh.indices);
}

XMMATRIX Renderable::GetWorld() const
{
	return XMMatrixTransformation(
		XMVECTOR(),
		XMVECTOR(),
		XMVectorSet(scale, scale, scale, 1.0),
		XMVECTOR(),
		XMLoadFloat4(&rotation),
		XMLoadFloat3(&position));
}

void Renderable::Bind(ID3D11DeviceContext * context)
{
	vb.Bind(context);
	ib.Bind(context);
}

unsigned int Renderable::GetNumIndices()
{
	return ib.GetNumIndices();
}

Material & Renderable::GetMat()
{
	return mat;
}

void Renderable::SetIsVisible(bool value)
{
	isVisible = value;
}

bool Renderable::GetIsVisible() const
{
	return isVisible;
}
