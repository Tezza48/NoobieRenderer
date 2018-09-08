#include "VertexBuffer.h"
#include <stdio.h>

VertexBuffer::VertexBuffer()
{

}

VertexBuffer::~VertexBuffer()
{

}

void VertexBuffer::Init(PtrDevice device, std::vector<Vertex> initialData)
{
	// Save the data for use later
	vertices = initialData;

	// fill out description for the buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * initialData.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = NULL;
	vbd.MiscFlags = NULL;
	vbd.StructureByteStride = 0;

	// Holds the data in a structure it can use
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices.data();// get the array from the vector

	// Tell the device to create the buffer
	D3D_CALL(device->CreateBuffer(&vbd, &data, buffer.GetAddressOf()));
}

void VertexBuffer::Bind(PtrContext context) const
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
}
