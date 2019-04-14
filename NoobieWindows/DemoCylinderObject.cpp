#include "DemoCylinderObject.h"
#include "ShapeGenerator.h"
#include "Input.h"

DemoCylinderObject::DemoCylinderObject(ID3D11Device * device) :
	Renderable(device, ShapeGenerator::GenerateCylinder(0.6f, 1.2f, 1.8f, 24, 4))
{

}

DemoCylinderObject::~DemoCylinderObject()
{

}

void DemoCylinderObject::Update(float dt, const Input & input)
{
	if (input.GetKey(Input::KB_W))
	{
		position.z += dt * 2.0f;
	}
	if (input.GetKey(Input::KB_S))
	{
		position.z -= dt * 2.0f;
	}
	if (input.GetKey(Input::KB_A))
	{
		position.x -= dt * 2.0f;
	}
	if (input.GetKey(Input::KB_D))
	{
		position.x += dt * 2.0f;
	}
}
