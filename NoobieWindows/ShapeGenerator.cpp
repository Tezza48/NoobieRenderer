#include "ShapeGenerator.h"

/* 
	For each x, y add a vertex to the list at position (x, 0, y)
	(planar in the up axis) offset so that it's centered around the
	middle of the plane.

	If it's not a far edge add indices to the incex list for the 4
	verts in te current quad. Quads grow towards Positive x and y.

	Keep track of the index of the Vert and index (currentQuad) 
	added to use when populating the index buffer.

	Initialize the Vertex and Index Buffers in the Mesh and return it
 */
Renderable ShapeGenerator::GeneratePlane(PtrDevice device, int width, int height, float scale)
{
	assert(width > 1);
	assert(height > 1);
	assert(scale != 0);
	Renderable output;
	vector<Vertex> vertices(width * height);
	vector<unsigned int> indices((width - 1) * (height - 1) * 6);
	unsigned int currentQuad = 0;
	unsigned int currentVert = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Vertex & currentVertex = vertices[y * width + x];
			float xf = static_cast<float>(x) - static_cast<float>(width - 1) / 2.0f;
			float yf = static_cast<float>(y) - static_cast<float>(height - 1) / 2.0f;

			currentVertex.position = XMFLOAT3(xf * scale, 0.0f, yf * scale);
			currentVertex.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			currentVertex.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			if (x < (width - 1) && y < (height - 1))
			{
				indices[currentQuad + 0] = currentVert;
				indices[currentQuad + 1] = currentVert + width;
				indices[currentQuad + 2] = currentVert + width + 1;

				indices[currentQuad + 3] = currentVert;
				indices[currentQuad + 4] = currentVert + width + 1;
				indices[currentQuad + 5] = currentVert + 1;
				currentQuad += 6;			
			}
			currentVert++;
		}
	}

	output.mesh.vb.Init(device, vertices);
	output.mesh.ib.Init(device, indices);

	output.transform.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	output.transform.rotation = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	output.transform.scale = 1.0f;

	return output;
}
