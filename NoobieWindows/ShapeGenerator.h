#pragma once
#include "Renderable.h"
#include "Utilities.h"

namespace ShapeGenerator
{
	// Width and height in verts
	Renderable GeneratePlane(PtrDevice device, int width, int height, float scale);
}