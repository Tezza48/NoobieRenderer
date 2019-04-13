#pragma once
#include <d3d11.h>
#include <vector>
#include "Utilities.h"

class IndexBuffer
{
private:
	std::vector<unsigned int> indices;// Cached index buffer
	ID3D11Buffer * buffer = NULL;

public:
	IndexBuffer();
	~IndexBuffer();

	void Init(ID3D11Device * device, std::vector<unsigned int> initialData);

	void Bind(ID3D11DeviceContext * context) const;

	unsigned int GetNumIndices() const { return indices.size(); }

	ID3D11Buffer * GetBuffer() const { return buffer; }
};

