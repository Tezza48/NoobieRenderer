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
	bool isVisible = true;
	VertexBuffer vb;
	IndexBuffer ib;
	Material mat;

protected:
	Renderable() {}

public:
	Renderable(ID3D11Device * device, MeshData mesh);

	XMMATRIX GetWorld() const;

	void Bind(ID3D11DeviceContext * context);

	void Update(float dt, const Input & input) override {}

	unsigned int GetNumIndices();
	Material & GetMat();

	void SetIsVisible(bool value);
	bool GetIsVisible() const;
};