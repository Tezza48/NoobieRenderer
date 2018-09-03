#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Renderer & renderer, Vertex * initialData, size_t count)
					: renderer(&renderer), vertices(initialData), numVertices(count)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = NULL;
	vbd.MiscFlags = NULL;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;

	D3D_CALL(this->renderer->GetDevice()->CreateBuffer(&vbd, &data, &buffer));
}

VertexBuffer::~VertexBuffer()
{
	buffer->Release();
	delete[] vertices;
}

void VertexBuffer::Bind()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	renderer->GetContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}
