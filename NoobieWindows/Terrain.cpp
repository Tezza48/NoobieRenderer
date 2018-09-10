#include "Terrain.h"

#include <math.h>

void Terrain::GenMesh(float * heightmap)
{
	// wrote algorithm in Monogame implementation at https://github.com/Tezza48/Monogame_TerrainGeneration
	vertices.resize(size * size);
	indices.resize((size - 1)* (size - 1) * 6);// wrote this for monogame terrain gen
	unsigned int quad = 0, lastVert = 0;
	for (size_t y = 0; y < size; y++)
	{
		for (size_t x = 0; x < size; x++)
		{
			float xf = static_cast<float>(x);
			float yf = static_cast<float>(y);
			float offset = static_cast<float>(size - 1) / 2.0f;
			vertices[y * size + x].position = XMFLOAT3(
				xf - offset,
				heightmap[y * size + x],
				yf - offset);

			vertices[y * size + x].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			
			//printf("v %f %f %f\n", vertices[y * size + x].position.x, vertices[y * size + x].position.y, vertices[y * size + x].position.z);

			// positional colouring
			vertices[y * size + x].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// bounds checking
			if (x < (size - 1) && y < (size - 1))
			{
				// indices for triangle 1
				indices[quad + 0] = lastVert;
				indices[quad + 1] = lastVert + size;
				indices[quad + 2] = lastVert + size + 1;
				//// indices for triangle 2
				indices[quad + 3] = lastVert;
				indices[quad + 4] = lastVert + size + 1;
				indices[quad + 5] = lastVert + 1;
				quad+=6;
			}
			lastVert ++;
		}
	}
	// Generate normals for verts not on the edge
	for (size_t y = 1; y < size - 1; y++)
	{
		for (size_t x = 1; x < size - 1; x++)
		{
			XMVECTOR vertPos = XMLoadFloat3(&vertices[y * size + x].position);
			XMVECTOR neighbours[4] = {
				vertPos - XMLoadFloat3(&vertices[(y + 1) * size + (x + 0)].position),
				vertPos - XMLoadFloat3(&vertices[(y + 0) * size + (x + 1)].position),
				vertPos - XMLoadFloat3(&vertices[(y - 1) * size + (x + 0)].position),
				vertPos - XMLoadFloat3(&vertices[(y + 0) * size + (x - 1)].position)
			};

			XMVECTOR normals[2] = {
				XMVector3Normalize(XMVector3Cross(neighbours[0], neighbours[1])),
				XMVector3Normalize(XMVector3Cross(neighbours[2], neighbours[3])),
			};

			XMVECTOR avgNormal = (normals[0] + normals[1]) / 2.0f;
			XMStoreFloat3(&vertices[y * size + x].normal, XMVector3Normalize(avgNormal));
		}
	}

	//for (size_t i = 0; i < indices.size(); i+=3)
	//{
	//	printf("f %u %u %u\n", indices[i], indices[i+1], indices[i+2]);
	//}

}

Terrain::~Terrain()
{
}

void Terrain::Init(PtrDevice device, float * heightmap)
{
	GenMesh(heightmap);

	// vertex Buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = NULL;
	vbd.MiscFlags = NULL;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vbInitialData;
	vbInitialData.pSysMem = vertices.data();

	D3D_CALL(device->CreateBuffer(&vbd, &vbInitialData, vertexBuffer.GetAddressOf()));

	// index Buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = NULL;
	ibd.MiscFlags = NULL;
	ibd.StructureByteStride = NULL;

	D3D11_SUBRESOURCE_DATA ibInitialData;
	ibInitialData.pSysMem = indices.data();

	D3D_CALL(device->CreateBuffer(&ibd, &ibInitialData, indexBuffer.GetAddressOf()));
}

void Terrain::Bind(PtrContext context)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
