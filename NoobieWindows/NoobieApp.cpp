#include "NoobieApp.h"
#include <DirectXColors.h>
#include <math.h>

NoobieApp::NoobieApp(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	: NoobieD3D(windowTitle, windowWidth, windowHeight)
{

}

NoobieApp::~NoobieApp()
{
	//fxWVP->Release();
}

void NoobieApp::Start()
{
	doVsync = false;
	enableMsaa = true;

	// Add renderables (objects) to the scene

	Renderable * plane = new Renderable(
		device, L"plane", ShapeGenerator::GeneratePlane(6, 6, 2.0f));
	plane->position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	renderables.push_back(plane);

	Renderable * cylinder = new Renderable(
		device, L"cylinder", ShapeGenerator::GenerateCylinder(1.0f, 1.0f, 4.0f, 16, 8));
	cylinder->position = XMFLOAT3(-2.0f, 0.0f, 0.0f);
	renderables.push_back(cylinder);

	Renderable * cone = new Renderable(
		device, L"cone", ShapeGenerator::GenerateCylinder(0.5f, 1.3f, 4.0f, 32, 1));
	cone->position = XMFLOAT3(2.0f, 0.0f, 0.0f);
	renderables.push_back(cone);

	for (auto obj : renderables)
	{
		wprintf(L"%s is at position: x=%.2f, y=%.2f, z=%.2f.\n",
			obj->name.c_str(), obj->position.x, obj->position.y, obj->position.z);
	}

	effect.Init(device);
	effect.SetTechnique("Default");

	XMVECTOR eyePos = XMVectorSet(0.0f, 8.0f, -5.0f, 1.0f);
	XMVECTOR target = XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f);
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
	XMVECTOR eyePos = XMVectorSet(sin(accTime * 0.5f) * 8.0f, 8.0f, cos(accTime * 0.5f) * 8.0f, 1.0f);
	XMVECTOR target = XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	view = XMMatrixLookAtLH(eyePos, target, up);
}

void NoobieApp::Draw(float dt)
{
	ClearBuffers(DirectX::Colors::DarkBlue);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	effect.Bind(device, context);

	XMMATRIX vp = view * proj;

	D3DX11_TECHNIQUE_DESC techDesc;

	auto technique = effect.SetTechnique("Default");
	technique->GetDesc(&techDesc);

	for (auto obj : renderables)
	{
		XMMATRIX wvp = obj->getWorld() * vp;

		obj->vb.Bind(context);
		obj->ib.Bind(context);

		effect.SetMatrix(effect.GetCBPerObject()->worldViewProj, &wvp);

		for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
		{
			technique->GetPassByIndex(pass)->Apply(0, context);

			context->DrawIndexed(obj->ib.GetNumIndices(), 0, 0);
		}
	}
}
