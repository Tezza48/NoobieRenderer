#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Renderer.h"

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	////XMFLOAT2 tex0;
	XMFLOAT3 normal;

	//Vertex(XMFLOAT3 pos, XMFLOAT4 color, XMFLOAT3 normal) : position(pos), normal(normal) {}
	//Vertex(float x, float y, float z) : position(XMFLOAT3(x, y, z)) {}
};

class VertexBuffer
{
private:
	std::vector<Vertex> vertices;
	ID3D11Buffer * buffer;
	ID3D11InputLayout * layout;

public:
	VertexBuffer();
	//VertexBuffer(Renderer & renderer, std::vector<Vertex> initialData);
	~VertexBuffer();

	void Init(Renderer * renderer, std::vector<Vertex> initialData);

	void Bind(Renderer * renderer) const;
	//void Unbind();
};

