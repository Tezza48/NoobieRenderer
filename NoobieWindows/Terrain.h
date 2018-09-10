#pragma once
#include "Utilities.h"
#include "Vertex.h"
#include <vector>

using std::vector;

class Terrain
{
private:
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	PtrBuffer vertexBuffer;
	PtrBuffer indexBuffer;

	unsigned int size;// width and height in quads
private:
	void GenMesh(float * heightmap);
public:
	Terrain() {};
	Terrain(unsigned int size) : size(size) { assert(size > 1); };
	~Terrain();

	void Init(PtrDevice device, float * heightmap);

	void Bind(PtrContext context);

	unsigned int GetNumIndices() const { return indices.size(); }
};

