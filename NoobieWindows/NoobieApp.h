#pragma once
#include "../NoobieCore/NoobieD3D.h"
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include "NoobieCore/Utilities.h"
#include "NoobieCore/Effect.h"
#include "NoobieCore/StandardEffect.h"
#include "NoobieCore/BaseObject.h"
#include "FlyCamera.h"

using namespace Noobie;
using namespace DirectX;
using Microsoft::WRL::ComPtr;


class NoobieApp :
	public NoobieD3D
{
private:
	vector<BaseObject *> scene;

	StandardEffect effect;

	float accTime = 0;

	FlyCamera * camera;

public:
	NoobieApp(string windowTitle, unsigned int windowWidth, unsigned int windowHeight);
	~NoobieApp();

private:
	void Start() override;
	bool Update(float dt) override;
	void Draw(float dt) override;
};

