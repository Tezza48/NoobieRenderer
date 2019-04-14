#pragma once
#include <DirectXMath.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MeshData.h"
#include "BaseObject.h"
#include "Material.h"

class Renderable: public BaseObject
{
private:
protected:
	VertexBuffer vb;
	IndexBuffer ib;
	Material mat;

protected:
	Renderable() {}

public:
	Renderable(ID3D11Device * device, MeshData mesh);

	inline XMMATRIX getWorld() { return XMMatrixTransformation(
		XMVECTOR(), 
		XMVECTOR(), 
		XMVectorSet(scale, scale, scale, 1.0), 
		XMVECTOR(), 
		XMLoadFloat4(&rotation), 
		XMLoadFloat3(&position)); }

	void Bind(ID3D11DeviceContext * context);

	void Update(float dt, const Input & input) override {}

	unsigned int GetNumIndices();
	Material & GetMat();
};