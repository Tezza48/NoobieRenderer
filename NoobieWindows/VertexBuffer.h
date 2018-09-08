#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Utilities.h"

using namespace DirectX;
using std::vector;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;
};

class VertexBuffer
{
private:
	vector<Vertex> vertices;// Cached vertex Buffer
	PtrBuffer buffer = NULL;
	//InputLayout layout;

public:
	VertexBuffer();
	~VertexBuffer();

	void Init(PtrDevice device, vector<Vertex> initialData);

	void Bind(PtrContext context) const;
};

