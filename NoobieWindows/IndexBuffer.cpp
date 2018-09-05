#include "IndexBuffer.h"
#include <stdio.h>

IndexBuffer::IndexBuffer(Renderer & renderer, std::vector<unsigned int> initialData)
	: indices(initialData)
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * initialData.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = NULL;
	ibd.MiscFlags = NULL;
	ibd.StructureByteStride = NULL;

	unsigned int * tempData = indices.data();

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = indices.data();

	D3D_CALL(renderer.GetDevice()->CreateBuffer(&ibd, &data, &buffer));
	printf("Creating IndexBuffer.\n");
}


IndexBuffer::~IndexBuffer()
{
	buffer->Release();
	buffer = nullptr;
	printf("Destroying IndexBuffer.\n");
}

void IndexBuffer::Bind(Renderer * renderer) const
{
	renderer->GetContext()->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}
