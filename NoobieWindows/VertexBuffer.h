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
	ID3D11Buffer * buffer = NULL;
	//InputLayout layout;

public:
	VertexBuffer();
	~VertexBuffer();

	void Init(ID3D11Device * device, vector<Vertex> initialData);

	void Bind(ID3D11DeviceContext * context) const;

	ID3D11Buffer * GetBuffer() const { return buffer; }
};

