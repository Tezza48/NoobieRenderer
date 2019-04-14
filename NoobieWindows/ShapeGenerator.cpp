#include "ShapeGenerator.h"
#define PI 3.141593f

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
MeshData ShapeGenerator::GeneratePlane(int width, int height, float scale)
{
	assert(width > 1);
	assert(height > 1);
	assert(scale != 0);
	MeshData mesh;
	mesh.vertices.resize(width * height);
	mesh.indices.resize((width - 1) * (height - 1) * 6);
	unsigned int currentQuad = 0;
	unsigned int currentVert = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Vertex & currentVertex = mesh.vertices[y * width + x];
			float xf = static_cast<float>(x) - static_cast<float>(width - 1) / 2.0f;
			float yf = static_cast<float>(y) - static_cast<float>(height - 1) / 2.0f;

			currentVertex.position = XMFLOAT3(xf * scale, 0.0f, yf * scale);
			currentVertex.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			currentVertex.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			if (x < (width - 1) && y < (height - 1))
			{
				mesh.indices[currentQuad + 0] = currentVert;
				mesh.indices[currentQuad + 1] = currentVert + width;
				mesh.indices[currentQuad + 2] = currentVert + width + 1;
				
				mesh.indices[currentQuad + 3] = currentVert;
				mesh.indices[currentQuad + 4] = currentVert + width + 1;
				mesh.indices[currentQuad + 5] = currentVert + 1;
				currentQuad += 6;			
			}
			currentVert++;
		}
	}

	return mesh;
}

MeshData ShapeGenerator::GenerateCylinder(float topRadius, float bottomRadius, float height, uint numSlices, uint numStacks)
{
	MeshData mesh;

	// top and bottom cap
	unsigned int numVerts = 2 * (numSlices + 1);
	unsigned int numIndices = 2 * (numSlices * 3);
	
	// sides
	numVerts += (numStacks + 1) * numSlices;
	numIndices += (numStacks + 1) * numSlices * 6;

	mesh.vertices.resize(numVerts);
	mesh.indices.resize(numIndices);

	float deltaAngle = 2 * PI / numSlices;
	float deltaRadius = (topRadius - bottomRadius) / numStacks;

	float stackHeight = height / numStacks;

	unsigned int numRings = numStacks + 1;

	for (uint ring = 0; ring < numRings; ring++)
	{
		float y = ring * stackHeight;
		float radius = bottomRadius + ring * deltaRadius;
		for (uint slice = 0; slice < numSlices; slice++)
		{
			// y * width + height
			mesh.vertices[ring * numSlices + slice] = {
				XMFLOAT3(sin(deltaAngle * slice) * radius, y, -cos(deltaAngle * slice) * radius), // Offset Later
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				XMFLOAT3(sin(deltaAngle * slice), 0.0f, -cos(deltaAngle * slice)) // Compute Later
			};

			if (ring  < numStacks)// no faces from the last edge
			{
				if (slice < numSlices - 1)
				{
					mesh.AddTriangle(
						 ring *		 numSlices +  slice,
						(ring + 1) * numSlices +  slice,
						(ring + 1) * numSlices + (slice + 1));
					mesh.AddTriangle(
						 ring *		 numSlices +  slice,
						(ring + 1) * numSlices + (slice + 1),
						 ring *		 numSlices + (slice + 1));
				}
				else
				{
					mesh.AddTriangle(
						 ring *		 numSlices + slice,
						(ring + 1) * numSlices + slice,
						(ring + 1) * numSlices + 0);
					mesh.AddTriangle(
						 ring *		 numSlices + slice,
						(ring + 1) * numSlices + 0,
						 ring *		 numSlices + 0);
				}
			}
		}
	}

	// Bottom Cap

	uint ringStart = numRings * numSlices + 1;// Skip one for the cap
	mesh.vertices[ringStart - 1] = {
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 
		XMFLOAT3(0.0f, -1.0f, 0.0f)
	};

	for (uint slice = 0; slice < numSlices; slice++)
	{
		mesh.vertices[ringStart + slice] = {
				XMFLOAT3(sin(deltaAngle * slice) * bottomRadius, 0, -cos(deltaAngle * slice) * bottomRadius), // Offset Later
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				XMFLOAT3(0.0f, -1.0f, 0.0f) // Compute Later
		};

		if (slice < numSlices - 1)
		{
			mesh.AddTriangle(
				ringStart - 1,
				ringStart + slice,
				ringStart + slice + 1);
		}
		else
		{
			mesh.AddTriangle(
				ringStart - 1,
				ringStart + slice,
				ringStart);
		}
	}

	// Top cap

	ringStart += numSlices + 1;

	mesh.vertices[ringStart - 1] = {
		XMFLOAT3(0.0f, height, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f)
	};

	for (uint slice = 0; slice < numSlices; slice++)
	{
		mesh.vertices[ringStart + slice] = {
				XMFLOAT3(sin(-deltaAngle * slice) * topRadius, height, -cos(-deltaAngle * slice) * topRadius), // Offset Later
				XMFLOAT4(1.0, 1.0f, 1.0f, 1.0f),
				XMFLOAT3(0.0f, 1.0f, 0.0f) // Compute Later
		};

		if (slice < numSlices - 1)
		{
			mesh.AddTriangle(
				ringStart - 1,
				ringStart + slice,
				ringStart + slice + 1);
		}
		else
		{
			mesh.AddTriangle(
				ringStart - 1,
				ringStart + slice,
				ringStart);
		}
	}
	//// center of cap
	//int centerIndex = 0;


	//// Bottom Cap
	//for (unsigned int offset = centerIndex + 1; offset < sliceCount + 1; offset++)
	//{
	//	Vertex & currentVert = mesh.vertices[offset];
	//	currentVert.position = XMFLOAT3(sin(deltaAngle * offset) * bottomRadius, 0.0f, cos(deltaAngle * offset) * bottomRadius);
	//	currentVert.normal = mesh.vertices[centerIndex].normal;
	//	currentVert.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//	if (offset == sliceCount) // if it's the last one end at the start of the circle
	//		mesh.AddTriangle(centerIndex, centerIndex + 1, centerIndex + offset);
	//	else
	//		mesh.AddTriangle(centerIndex, centerIndex + offset + 1, centerIndex + offset);
	//}

	//centerIndex += sliceCount + 1; // Next vert
	//// Top Cap
	//mesh.vertices[centerIndex] = {
	//	XMFLOAT3(0.0f, height, 0.0f), 
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 
	//	XMFLOAT3(0.0f, 1.0f, 0.0f)
	//};

	//for (unsigned int offset = 1; offset < sliceCount + 1; offset++)
	//{
	//	Vertex & currentVert = mesh.vertices[centerIndex + offset];
	//	currentVert.position = XMFLOAT3(sin(deltaAngle * offset) * topRadius, 0.0f, cos(deltaAngle * offset) * topRadius);
	//	currentVert.normal = mesh.vertices[centerIndex].normal;
	//	currentVert.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//	if (offset == sliceCount) // if it's the last one end at the start of the circle
	//		mesh.AddTriangle(centerIndex, centerIndex + offset, centerIndex + 1);
	//	else
	//		mesh.AddTriangle(centerIndex, centerIndex + offset, centerIndex + offset + 1);
	//}

	return mesh;

}
