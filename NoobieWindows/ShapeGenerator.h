#pragma once
#include "NoobieCore/MeshData.h"
#include "NoobieCore/Utilities.h"

namespace ShapeGenerator
{
	// Width and height in verts
	MeshData GeneratePlane(int width, int height, float scale);
	MeshData GenerateCylinder(float topRadius, float bottomRadius, float height, unsigned int sliceCount, unsigned int stackCount);
}