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

	ID3D11ShaderResourceView* diffTexSRV;

protected:
	Renderable() {};

public:
	Renderable(const char * name, ID3D11Device * device, MeshData mesh);
	~Renderable();

	XMFLOAT4X4 GetWorld() const;

	void Bind(ID3D11DeviceContext * context);

	void Update(float dt) override {}

	unsigned int GetNumIndices();
	Material & GetMat();
	ID3D11ShaderResourceView* GetTexture();

	void SetIsVisible(bool value);
	bool GetIsVisible() const;
};