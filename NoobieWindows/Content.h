#pragma once
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Content
{
	void LoadOBJ(Renderer * renderer, std::string filename, VertexBuffer & vBuffer, IndexBuffer & iBuffer, float scale = 1.0f);
}