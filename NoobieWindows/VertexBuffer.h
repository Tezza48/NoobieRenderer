#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Renderer.h"

struct Vertex
{
	DirectX::XMFLOAT3 position;
	//XMFLOAT4 color;
	////XMFLOAT2 tex0;
	//XMFLOAT3 normal;
};

class VertexBuffer
{
private:
	std::vector<Vertex> vertices;
	ID3D11Buffer * buffer;

public:
	VertexBuffer() {}
	VertexBuffer(Renderer & renderer, std::vector<Vertex> initialData);
	~VertexBuffer();

	void Bind(Renderer & renderer) const;
	//void Unbind();
};

