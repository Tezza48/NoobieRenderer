#pragma once
#include "Renderable.h"
class DemoCylinderObject :
	public Renderable
{
public:
	DemoCylinderObject(ID3D11Device * device);
	~DemoCylinderObject();

	void Update(float dt, const Input & input) override;
};

