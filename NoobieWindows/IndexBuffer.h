#pragma once
#include <d3d11.h>
#include "Renderer.h"

class IndexBuffer
{
private:
	Renderer * renderer;
	size_t numIndices;
	unsigned int * indices;
	ID3D11Buffer * buffer;

public:
	IndexBuffer(Renderer & renderer, unsigned int * initialData, size_t count);
	~IndexBuffer();

	void Bind();

	size_t GetNumIndices() const { return numIndices; }
};

