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
	//Content::LoadOBJ(device, "res/model/bunny.obj", bunny.vb, bunny.ib, 10.0f);

	terrain.Init(device);

	effect.Init(device);
	effect.SetTechnique("TerrainTech");

	//bunny.position = XMVectorSet(0.0f, -1.0f, 0.0f, 1.0f);
	XMVECTOR eyePos = XMVectorSet(0.0f, 30.0f, -50.0f, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	
	view = XMMatrixLookAtLH(eyePos, target, up);
	proj = XMMatrixPerspectiveFovLH(XM_PI / 4, GetAspectRatio(), 0.1f, 1000.0f);

}

void NoobieApp::Update(float dt)
{
	camTimer +=dt;
	XMVECTOR eyePos = XMVectorSet(0.0f, sin(camTimer) * 20.0f, -30.0f, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	//view = XMMatrixLookAtLH(eyePos, target, up);
}

void NoobieApp::Draw(float dt)
{
	ClearBuffers(DirectX::Colors::CornflowerBlue);
	//XMMATRIX wvp = XMMatrixTranslationFromVector(bunny.position) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), bunny.rotation) * view * proj;
	XMMATRIX wvp = view * proj;
	effect.SetMatrix(effect.GetCBPerObject().worldViewProj, &wvp);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	effect.Bind(device, context);

	terrain.Bind(context);
	//bunny.vb.Bind(context);
	//bunny.ib.Bind(context);

	D3DX11_TECHNIQUE_DESC techDesc;
	effect.GetCurrentTechnique()->GetDesc(&techDesc);

	for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
	{
		effect.GetCurrentTechnique()->GetPassByIndex(pass)->Apply(0, context.Get());

		context->DrawIndexed(terrain.GetNumIndices(), 0, 0);
	}

}
