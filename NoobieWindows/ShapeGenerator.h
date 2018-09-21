#pragma once
#include "Common.h"
#include "MeshData.h"
#include "Utilities.h"

namespace ShapeGenerator
{
	// Width and height in verts
	MeshData GeneratePlane(int width, int height, float scale);
	MeshData GenerateCylinder(float topRadius, float bottomRadius, float height, uint sliceCount, uint stackCount);
}