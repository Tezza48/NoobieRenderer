#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Utilities.h"
#include "Vertex.h"

using namespace DirectX;
using std::vector;

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

