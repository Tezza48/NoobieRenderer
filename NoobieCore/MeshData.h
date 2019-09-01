#pragma once
#include <vector>
#include "Vertex.h"
#include <string>

using std::vector;

struct MeshData
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	unsigned int currentTri;

	struct TextureData
	{
		unsigned char* data;
		int w, h, bpp;

		TextureData() : data(nullptr), w(0), h(0), bpp(0) {}
		TextureData(unsigned char * data, int w, int h, int bpp) : 
			data(data), w(w), h(h), bpp(bpp) {}
	};

	std::wstring diffuse;
	//TextureData specular;
	//TextureData normal;

	MeshData() : currentTri(0), diffuse(L"") {}

	void AddTriangle(int A, int B, int C)
	{
		indices[currentTri + 0] = A;
		indices[currentTri + 1] = B;
		indices[currentTri + 2] = C;
		currentTri += 3;
	}
};