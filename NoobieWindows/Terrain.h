#pragma once
#include <d3d11.h>
#include "Vertex.h"
#include <vector>

using std::vector;

class Terrain
{
private:
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;

	unsigned int size;// width and height in quads
private:
	void GenMesh(float * heightmap);
public:
	Terrain() {};
	Terrain(unsigned int size) : size(size) { assert(size > 1); };
	~Terrain();

	void Init(ID3D11Device * device, float * heightmap);

	void Bind(ID3D11DeviceContext * context);

	unsigned int GetNumIndices() const { return indices.size(); }
};

