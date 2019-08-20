#pragma once
#include <vector>
#include <d3d11.h>
#include "NoobieCore/Vertex.h"
#include "NoobieCore/Renderable.h"

using std::vector;

class Terrain: public Renderable
{
private:
	MeshData mesh;

	unsigned int size;// width and height in quads
private:
	void GenMesh(float * heightmap);
public:
	Terrain(ID3D11Device * device, float * heightmap, unsigned int size);
	Terrain(unsigned int size) : size(size) { assert(size > 1); };
	~Terrain();
};

