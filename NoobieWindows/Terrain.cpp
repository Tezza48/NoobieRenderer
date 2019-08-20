#include "Terrain.h"
#include "NoobieCore/Utilities.h"
#include <math.h>

Terrain::~Terrain()
{

}

Terrain::Terrain(ID3D11Device * device, float * heightmap, unsigned int size): size(size)
{
	GenMesh(heightmap);
	vb.Init(device, mesh.vertices);
	ib.Init(device, mesh.indices);
}

void Terrain::GenMesh(float * heightmap)
{
	mesh.vertices.resize(size * size);
	mesh.indices.resize((size - 1)* (size - 1) * 6);
	unsigned int quad = 0, lastVert = 0;
	for (size_t y = 0; y < size; y++)
	{
		for (size_t x = 0; x < size; x++)
		{
			float xf = static_cast<float>(x);
			float yf = static_cast<float>(y);
			float offset = static_cast<float>(size - 1) / 2.0f;
			mesh.vertices[y * size + x].position = XMFLOAT3(
				xf - offset,
				heightmap[y * size + x],
				yf - offset);

			mesh.vertices[y * size + x].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			mesh.vertices[y * size + x].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			if (x < (size - 1) && y < (size - 1))
			{
				// indices for triangle 1
				mesh.indices[quad + 0] = lastVert;
				mesh.indices[quad + 1] = lastVert + size;
				mesh.indices[quad + 2] = lastVert + size + 1;
				//// indices for triangle 2
				mesh.indices[quad + 3] = lastVert;
				mesh.indices[quad + 4] = lastVert + size + 1;
				mesh.indices[quad + 5] = lastVert + 1;
				quad += 6;
			}
			lastVert++;
		}
	}

	// Generate normals for verts not on the edge
	for (size_t y = 1; y < size - 1; y++)
	{
		for (size_t x = 1; x < size - 1; x++)
		{
			XMVECTOR vertPos = XMLoadFloat3(&mesh.vertices[y * size + x].position);
			XMVECTOR neighbours[4] = {
				vertPos - XMLoadFloat3(&mesh.vertices[(y + 1) * size + (x + 0)].position),
				vertPos - XMLoadFloat3(&mesh.vertices[(y + 0) * size + (x + 1)].position),
				vertPos - XMLoadFloat3(&mesh.vertices[(y - 1) * size + (x + 0)].position),
				vertPos - XMLoadFloat3(&mesh.vertices[(y + 0) * size + (x - 1)].position)
			};

			XMVECTOR normals[2] = {
				XMVector3Normalize(XMVector3Cross(neighbours[0], neighbours[1])),
				XMVector3Normalize(XMVector3Cross(neighbours[2], neighbours[3])),
			};

			XMVECTOR avgNormal = (normals[0] + normals[1]) / 2.0f;
			XMStoreFloat3(&mesh.vertices[y * size + x].normal, XMVector3Normalize(avgNormal));
		}
	}
}