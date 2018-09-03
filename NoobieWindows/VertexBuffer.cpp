#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Renderer & renderer, std::vector<Vertex> initialData)
					: vertices(initialData)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * initialData.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = NULL;
	vbd.MiscFlags = NULL;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices.data();// get the array from the vector

	D3D_CALL(renderer.GetDevice()->CreateBuffer(&vbd, &data, &buffer));
}

VertexBuffer::~VertexBuffer()
{
	buffer->Release();
	buffer = nullptr;
}

void VertexBuffer::Bind(Renderer & renderer) const
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	renderer.GetContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}
