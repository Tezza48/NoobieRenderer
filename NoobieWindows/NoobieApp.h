#pragma once
#include "NoobieD3D.h"
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <wrl.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

struct Renderable
{
	XMVECTOR position;
	VertexBuffer vb;
	IndexBuffer ib;
};

class NoobieApp :
	public NoobieD3D
{
private:
	Renderable Bunny;
	ComPtr<ID3DX11Effect> effect;
	ComPtr<ID3DX11EffectTechnique> currentTech;
public:
	NoobieApp(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight) : NoobieD3D(windowTitle, windowWidth, windowHeight) {};
	~NoobieApp();


private:
	void Start() override;
	void Update() override;
	void Draw() override;
};

