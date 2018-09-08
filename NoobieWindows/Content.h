#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Utilities.h"

namespace Content
{
	void LoadOBJ(PtrDevice device, std::string filename, VertexBuffer & vBuffer, IndexBuffer & iBuffer, float scale = 1.0f);
}