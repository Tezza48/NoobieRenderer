#include "DemoPlaneObject.h"
#include "ShapeGenerator.h"

DemoPlaneObject::DemoPlaneObject(ID3D11Device * device):
	Renderable(device, ShapeGenerator::GeneratePlane(10, 10, 5.0f))
{
}

DemoPlaneObject::~DemoPlaneObject()
{
}
