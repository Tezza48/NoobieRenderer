#pragma once
#include <DirectXMath.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;

struct Renderable
{
	struct Transform
	{
		XMFLOAT3 position;
		XMFLOAT4 rotation;
		float scale = 10.0f;
	};

	struct Mesh
	{
		VertexBuffer vb;
		IndexBuffer ib;
	};

	Transform transform;
	Mesh mesh;
};