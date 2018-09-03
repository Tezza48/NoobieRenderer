#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Renderer &renderer, unsigned int * initialData, size_t count)
	: renderer(&renderer), indices(initialData), numIndices(count)
{

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * numIndices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = NULL;
	ibd.MiscFlags = NULL;
	ibd.StructureByteStride = NULL;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = indices;

	D3D_CALL(this->renderer->GetDevice()->CreateBuffer(&ibd, &data, &buffer));
}


IndexBuffer::~IndexBuffer()
{
	buffer->Release();
	delete[] indices;
}

void IndexBuffer::Bind()
{
	renderer->GetContext()->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}
