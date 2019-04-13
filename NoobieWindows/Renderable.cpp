#include "Renderable.h"

Renderable::Renderable(ID3D11Device * device, std::wstring name, MeshData mesh) :
	name(name), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f, 1.0f), scale(1.0)
{
	vb.Init(device, mesh.vertices);
	vb.GetBuffer()->SetPrivateData(WKPDID_D3DDebugObjectNameW, name.length(), name.c_str());

	ib.Init(device, mesh.indices);
	ib.GetBuffer()->SetPrivateData(WKPDID_D3DDebugObjectNameW, name.length(), name.c_str());
}