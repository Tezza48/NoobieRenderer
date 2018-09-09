#pragma once
#include "NoobieD3D.h"
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Utilities.h"
#include "Effect.h"

using namespace Noobie;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct Renderable
{
	XMVECTOR position;
	float rotation;
	VertexBuffer vb;
	IndexBuffer ib;
};

struct Camera
{
	XMVECTOR eyePos;
	XMMATRIX view;
	XMMATRIX projection;
};

class NoobieApp :
	public NoobieD3D
{
private:
	Renderable bunny;
	
	Effect effect;

	XMMATRIX view, proj;

public:
	NoobieApp(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight)
		: NoobieD3D(windowTitle, windowWidth, windowHeight) {};
	~NoobieApp();

private:
	void Start() override;
	void Update(float dt) override;
	void Draw(float dt) override;
};

