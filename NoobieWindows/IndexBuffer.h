#pragma once
#include <d3d11.h>
#include <vector>
#include "Utilities.h"

class IndexBuffer
{
private:
	std::vector<unsigned int> indices;// Cached index buffer
	PtrBuffer buffer = NULL;

public:
	IndexBuffer();
	~IndexBuffer();

	void Init(PtrDevice device, std::vector<unsigned int> initialData);

	void Bind(PtrContext context) const;

	unsigned int GetNumIndices() const { return indices.size(); }
};

