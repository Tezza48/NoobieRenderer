#include "Renderable.h"

Renderable::Renderable(ID3D11Device * device, MeshData mesh) : BaseObject()
{
	vb.Init(device, mesh.vertices);
	ib.Init(device, mesh.indices);
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
