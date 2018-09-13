#include "NoobieApp.h"
#include <DirectXColors.h>
#include "Content.h"
#include <math.h>

NoobieApp::NoobieApp(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	: NoobieD3D(windowTitle, windowWidth, windowHeight)
{
	terrain = Terrain(50);
}

NoobieApp::~NoobieApp()
{
	//fxWVP->Release();
}

void NoobieApp::Start()
{
	Content::LoadOBJ(device, "res/model/bunny.obj", bunny.vb, bunny.ib, 10.0f);
	bunnyTransform.position = XMFLOAT3(0.0f, -1.0f, 0.0f);

	vector<float> heightmap;
	heightmap.reserve(50 * 50);

	for (size_t y = 0; y < 50; y++)
	{
		for (size_t x = 0; x < 50; x++)
		{
			heightmap.emplace_back((sin(x / 25.0f * 4) + cos(y / 25.0f * 4)) * 5.0f);
		}
	}

	terrain.Init(device, heightmap.data());
	terrainTransform.position = XMFLOAT3(0.0f, 0.0f, 0.0f);

	effect.Init(device);
	effect.SetTechnique("TerrainTech");

	XMVECTOR eyePos = XMVectorSet(0.0f, 30.0f, -50.0f, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	
	view = XMMatrixLookAtLH(eyePos, target, up);
	proj = XMMatrixPerspectiveFovLH(XM_PI / 4, GetAspectRatio(), 0.1f, 1000.0f);

}

void NoobieApp::Update(float dt)
{
	if (input.GetKey(Input::KB_ESCAPE))
	{
		Quit();
		return;
	}
	accTime +=dt;
	XMVECTOR eyePos = XMVectorSet(0.0f, sin(accTime) * 20.0f, -30.0f, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	//view = XMMatrixLookAtLH(eyePos, target, up);

	if (input.GetKey(Input::KB_A))
		bunnyTransform.position.x -= 10 * dt;
	if (input.GetKey(Input::KB_D))
		bunnyTransform.position.x +=  10 * dt;

	//bunnyTransform.position.z = cos(accTime / 2.0f) * 10;
	//bunnyTransform.position.y = sin(accTime * 2.0f) * 10;
}

void NoobieApp::Draw(float dt)
{
	ClearBuffers(DirectX::Colors::CornflowerBlue);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	effect.Bind(device, context);

	XMMATRIX vp = view * proj;
	XMMATRIX wvpBunny = XMMatrixScaling(bunnyTransform.scale, bunnyTransform.scale, bunnyTransform.scale) * XMMatrixTranslationFromVector(XMLoadFloat3(&bunnyTransform.position)) * vp;
	XMMATRIX wvpTerrain = XMMatrixTranslationFromVector(XMLoadFloat3(&terrainTransform.position)) * vp;

	terrain.Bind(context);
	effect.SetMatrix(effect.GetCBPerObject().worldViewProj, &wvpTerrain);

	D3DX11_TECHNIQUE_DESC techDesc;
	// Terrain technique
	effect.SetTechnique("TerrainTech")->GetDesc(&techDesc);

	for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
	{
		effect.GetCurrentTechnique()->GetPassByIndex(pass)->Apply(0, context.Get());

		context->DrawIndexed(terrain.GetNumIndices(), 0, 0);
	}

	bunny.vb.Bind(context);
	bunny.ib.Bind(context);
	effect.SetMatrix(effect.GetCBPerObject().worldViewProj, &wvpBunny);
	effect.SetTechnique("NormalTech")->GetDesc(&techDesc);

	for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
	{
		effect.GetCurrentTechnique()->GetPassByIndex(pass)->Apply(0, context.Get());

		context->DrawIndexed(bunny.ib.GetNumIndices(), 0, 0);
	}

}
