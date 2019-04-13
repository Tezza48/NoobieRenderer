#pragma once
#include "NoobieD3D.h"
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include "Utilities.h"
#include "Effect.h"
#include "StandardEffect.h"
#include "Renderable.h"
#include "ShapeGenerator.h"

using namespace Noobie;
using namespace DirectX;
using Microsoft::WRL::ComPtr;


class NoobieApp :
	public NoobieD3D
{
private:
	vector<Renderable *> renderables;
	
	StandardEffect effect;

	float accTime = 0;

	XMMATRIX view, proj;
	XMVECTOR eyePosW;

public:
	NoobieApp(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight);
	~NoobieApp();

private:
	void Start() override;
	void Update(float dt) override;
	void Draw(float dt) override;
};

