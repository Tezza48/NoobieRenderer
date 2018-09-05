#pragma once
#include <d3d11.h>
#include <vector>
#include "Renderer.h"

class IndexBuffer
{
private:
	std::vector<unsigned int> indices;
	ID3D11Buffer * buffer;

public:
	IndexBuffer();
	~IndexBuffer();

	void Init(Renderer * renderer, std::vector<unsigned int> initialData);

	void Bind(Renderer * renderer) const;

	unsigned int GetNumIndices() const { return indices.size(); }
};

