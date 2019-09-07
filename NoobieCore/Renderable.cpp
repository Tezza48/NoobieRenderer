#include "Renderable.h"
#include <D3DX11tex.h>

using namespace DirectX;

Renderable::Renderable(const char * name, ID3D11Device * device, MeshData mesh) : BaseObject()
{
	position = {};

	XMStoreFloat4(&rotation, XMQuaternionIdentity());
	vb.Init(device, mesh.vertices);
	ib.Init(device, mesh.indices);

	diffTexSRV = nullptr;

	if (mesh.diffuse != L"") {
		D3D_CALL(D3DX11CreateShaderResourceViewFromFile(device, mesh.diffuse.c_str(), 0, 0, &diffTexSRV, 0));
	}
}

Renderable::~Renderable()
{
	SafeRelease(diffTexSRV);
}

XMFLOAT4X4 Renderable::GetWorld() const
{
	XMFLOAT4X4 world;
	
	XMStoreFloat4x4(&world, XMMatrixTransformation(
		XMVECTOR(),
		XMVECTOR(),
		XMVectorSet(scale, scale, scale, 1.0),
		XMVECTOR(),
		XMLoadFloat4(&rotation),
		XMLoadFloat3(&position)));

	return world;
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

ID3D11ShaderResourceView* Renderable::GetTexture()
{
	return diffTexSRV;
}

void Renderable::SetIsVisible(bool value)
{
	isVisible = value;
}

bool Renderable::GetIsVisible() const
{
	return isVisible;
}
