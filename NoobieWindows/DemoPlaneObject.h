#pragma once
#include "Renderable.h"
class DemoPlaneObject :
	public Renderable
{
public:
	DemoPlaneObject(ID3D11Device * device);
	~DemoPlaneObject();

	void Update(float dt, const Input & input) override {}
};

