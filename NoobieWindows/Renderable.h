#pragma once
#include <DirectXMath.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MeshData.h"

struct Renderable
{
	Renderable(ID3D11Device * device, std::wstring name, MeshData mesh);

	std::wstring name;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 rotation;
	float scale = 10.0f;

	VertexBuffer vb;
	IndexBuffer ib;

	inline XMMATRIX getWorld() { return XMMatrixTransformation(
		XMVECTOR(), 
		XMVECTOR(), 
		XMVectorSet(scale, scale, scale, 1.0), 
		XMVECTOR(), 
		XMLoadFloat4(&rotation), 
		XMLoadFloat3(&position)); }
};