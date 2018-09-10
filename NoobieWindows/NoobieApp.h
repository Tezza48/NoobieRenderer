#pragma once
#include "NoobieD3D.h"
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Utilities.h"
#include "Effect.h"
#include "Terrain.h"

using namespace Noobie;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

//struct Entity
//{
//	
//};
//
//class Behaviour
//{
//public:
//	Behaviour();
//	virtual ~Behaviour();
//};

struct Transform
{
	XMFLOAT3 position;
	XMFLOAT4 rotation;
	float scale = 10.0f;
};

struct Mesh
{
	VertexBuffer vb;
	IndexBuffer ib;
};

class NoobieApp :
	public NoobieD3D
{
private:
	Mesh bunny;
	Transform bunnyTransform;
	
	Terrain terrain;
	Transform terrainTransform;
	
	Effect effect;

	float accTime = 0;

	XMMATRIX view, proj;

public:
	NoobieApp(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight);
	~NoobieApp();

private:
	void Start() override;
	void Update(float dt) override;
	void Draw(float dt) override;
};

