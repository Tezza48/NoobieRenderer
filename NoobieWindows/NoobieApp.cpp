#include "NoobieApp.h"
#include <DirectXColors.h>
#include "Content.h"
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
	Renderable newRenderable;
	MeshData newMesh;

	//{
	//	newMesh = ShapeGenerator::GeneratePlane(6, 3, 2.0f);
	//	newRenderable.mesh.vb.Init(device, newMesh.vertices);
	//	newRenderable.mesh.ib.Init(device, newMesh.indices);
	//	newRenderable.transform = {XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f), 0.0f};

	//	renderables.push_back(newRenderable);
	//}

	{
		newMesh = ShapeGenerator::GenerateCylinder(1.0f, 1.0f, 4.0f, 16, 8);
		newRenderable.mesh.vb.Init(device, newMesh.vertices);
		newRenderable.mesh.ib.Init(device, newMesh.indices);
		newRenderable.transform = { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f), 0.0f };

		renderables.push_back(newRenderable);
	}



	effect.Init(device);
	effect.SetTechnique("TerrainTech");

	XMVECTOR eyePos = XMVectorSet(0.0f, 8.0f, -10.0f, 1.0f);
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
	XMVECTOR eyePos = XMVectorSet(0.0f, sin(accTime) * 8.0f, -10.0f, 1.0f);
	XMVECTOR target = XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	view = XMMatrixLookAtLH(eyePos, target, up);


}

void NoobieApp::Draw(float dt)
{
	ClearBuffers(DirectX::Colors::CornflowerBlue);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	effect.Bind(device, context);

	XMMATRIX vp = view * proj;

	D3DX11_TECHNIQUE_DESC techDesc;

	effect.SetTechnique("TerrainTech")->GetDesc(&techDesc);

	for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
	{
		effect.GetCurrentTechnique()->GetPassByIndex(pass)->Apply(0, context.Get());

		for each (Renderable obj in renderables)
		{
			XMMATRIX wvp = XMMatrixTranslationFromVector(XMLoadFloat3(&obj.transform.position)) * view * proj;
			effect.SetMatrix(effect.GetCBPerObject().worldViewProj, &wvp);
			obj.mesh.vb.Bind(context);
			obj.mesh.ib.Bind(context);
			context->DrawIndexed(obj.mesh.ib.GetNumIndices(), 0, 0);
		}

	}

}
