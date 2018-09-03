#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
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
	Renderer * renderer;

	size_t numVertices;
	Vertex *vertices;
	ID3D11Buffer * buffer;

public:
	VertexBuffer(Renderer & renderer, Vertex * initialData, size_t count);
	~VertexBuffer();

	void Bind();
	//void Unbind();
};

