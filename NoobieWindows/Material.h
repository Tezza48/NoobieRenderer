#pragma once
#include <DirectXMath.h>

struct Material
{
	Material()
	{
		diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		reflect = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT4 reflect;
};