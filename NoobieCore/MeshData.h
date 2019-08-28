#pragma once
#include <vector>
#include "Vertex.h"

using std::vector;

struct MeshData
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int currentTri = 0;

	void AddTriangle(int A, int B, int C)
	{
		indices[currentTri + 0] = A;
		indices[currentTri + 1] = B;
		indices[currentTri + 2] = C;
		currentTri += 3;
	}
};