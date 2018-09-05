#include "IndexBuffer.h"
#include <stdio.h>

IndexBuffer::IndexBuffer()
{
	buffer = nullptr;
}


IndexBuffer::~IndexBuffer()
{
	buffer->Release();
	buffer = nullptr;
}

void IndexBuffer::Init(Renderer * renderer, std::vector<unsigned int> initialData)
{
	// See VertexBuffer.cpp for comments
	indices = initialData;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * initialData.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = NULL;
	ibd.MiscFlags = NULL;
	ibd.StructureByteStride = NULL;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = indices.data();

	D3D_CALL(renderer->GetDevice()->CreateBuffer(&ibd, &data, &buffer));
}

void IndexBuffer::Bind(Renderer * renderer) const
{
	renderer->GetContext()->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}
