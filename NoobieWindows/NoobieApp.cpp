#include "NoobieApp.h"
#include <DirectXColors.h>
#include <math.h>
#include "DemoPlaneObject.h"
#include "DemoCylinderObject.h"
#include "Assets.h"
#include "Terrain.h"

NoobieApp::NoobieApp(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	: NoobieD3D(windowTitle, windowWidth, windowHeight)
{
	cone = nullptr;
}

NoobieApp::~NoobieApp()
{
	for (auto & baseObj : scene)
	{
		if (baseObj)
		{
			delete baseObj;
			baseObj = nullptr;
		}
	}
	//fxWVP->Release();
}

void NoobieApp::Start()
{
	doVsync = true;
	enableMsaa = true;

	// Add renderables (objects) to the scene
	//auto plane = new DemoPlaneObject(device);
	//scene.push_back(plane);
	//renderables.push_back(plane);
	//plane->GetMat().diffuse = XMFLOAT4(0.96f, 0.1f, 0.1f, 1.0f);
	//plane->GetMat().specular = XMFLOAT4(0.96f, 0.1f, 0.1f, 10.0f);

	float heightmap[24 * 24];

	for (size_t y = 0; y < 24; y++)
	{
		for (size_t x = 0; x < 24; x++)
		{
			heightmap[y * 24 + x] = sin((float)x) + cos((float)y);
		}
	}

	auto terrain = new Terrain(device, heightmap, 24);
	scene.push_back(terrain);
	renderables.push_back(terrain);
	terrain->GetMat().diffuse = XMFLOAT4(0.2f, 0.8f, 0.1f, 1.0f);
	terrain->GetMat().specular = XMFLOAT4(0.2f, 0.8f, 0.1f, 1.0f);

	cone = new DemoCylinderObject(device);
	scene.push_back(cone);
	updatables.push_back(cone);
	renderables.push_back(cone);

	auto bunny = new Renderable(device, Assets::LoadObj(Assets::ModelEnum::BUNNY_OBJ, 10.0f));
	scene.push_back(bunny);
	renderables.push_back(bunny);

	cockpit = new Renderable(device, Assets::LoadObj(Assets::ModelEnum::COCKPIT_OBJ, 1.0f));
	scene.push_back(cockpit);
	renderables.push_back(cockpit);
	updatables.push_back(cockpit);
	cockpit->GetMat().diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	cockpit->GetMat().specular = XMFLOAT4(0.2f, 0.2f, 0.5f, 1000.0f);

	//Renderable * cone = new Renderable(
	//	device, L"cone", ShapeGenerator::GenerateCylinder(0.5f, 1.3f, 4.0f, 32, 1));
	//cone->position = XMFLOAT3(2.0f, 0.0f, 0.0f);
	//renderables.push_back(cone);

	effect.Init(device);
	effect.SetTechnique("Default");

	XMVECTOR eyePos = XMVectorSet(0.0f, 8.0f, -5.0f, 1.0f);
	XMVECTOR target = XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	
	view = XMMatrixLookAtLH(eyePos, target, up);
}

void NoobieApp::Update(float dt)
{
	if (input.GetKey(Input::KB_ESCAPE))
	{
		Quit();
		return;
	}
	accTime +=dt;
	eyePosW = XMVectorSet(sin(accTime * 0.5f) * 8.0f, 5.0f, cos(accTime * 0.5f) * 8.0f, 1.0f);
	//XMVECTOR target = XMVectorSet(0.0f, 2.0f, 0.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	XMVECTOR target = XMLoadFloat3(&cone->GetPosition());

	view = XMMatrixLookAtLH(eyePosW, target, up);
	if (hasResized)
	{
		proj = XMMatrixPerspectiveFovLH(XM_PI / 2, GetAspectRatio(), 0.1f, 1000.0f);
	}

	for (const auto obj : updatables)
	{
		obj->Update(dt, input);
	}
	XMFLOAT3 cpPos;
	XMStoreFloat3(&cpPos, eyePosW);
	cockpit->SetPosition(cpPos);

	XMVECTOR forwardV = XMVector3Normalize(eyePosW - target);
	XMVECTOR rightV = XMVector3Normalize(XMVector3Cross(up, forwardV));
	XMVECTOR upV = XMVector3Normalize(XMVector3Cross(forwardV, rightV));

	XMFLOAT3 fwd;
	XMStoreFloat3(&fwd, forwardV);
	XMFLOAT3 right;
	XMStoreFloat3(&right, rightV);
	XMFLOAT3 upf;
	XMStoreFloat3(&upf, upV);

	auto look = XMMatrixSet(
		right.x, right.y, right.z, 0.0f,
		upf.x, upf.y, upf.z, 0.0f,
		fwd.x, fwd.y, fwd.z, 0.0f,
		cpPos.x, cpPos.x, cpPos.x, 1.0);

	auto rot = XMQuaternionRotationMatrix(look);
	XMFLOAT4 quat;
	XMStoreFloat4(&quat, rot);
	cockpit->SetRotation(quat);


}

void NoobieApp::Draw(float dt)
{
	XMFLOAT4 ambient = XMFLOAT4(0.1f, 0.1f, 0.2f, 1.0f);
	ClearBuffers(&ambient.x);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	effect.Bind(device, context);

	XMMATRIX vp = view * proj;

	D3DX11_TECHNIQUE_DESC techDesc;

	auto technique = effect.GetCurrentTechnique();
	technique->GetDesc(&techDesc);

	auto ambientVector = XMLoadFloat4(&ambient);
	effect.SetVector(effect.getPerFrame()->ambientLight, &ambientVector);

	for (auto obj : renderables)
	{
		auto world = obj->getWorld();
		XMMATRIX wvp = world * vp;
		XMMATRIX worldInvTrans;
		{
			auto w = world;
			w.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			XMVECTOR det = XMMatrixDeterminant(w);
			worldInvTrans = XMMatrixTranspose(XMMatrixInverse(&det, w));
		}

		obj->Bind(context);

		effect.SetMatrix(effect.getPerObject()->worldViewProj, &wvp);
		effect.SetMatrix(effect.getPerObject()->world, &world);
		effect.SetMatrix(effect.getPerObject()->worldInverseTranspose, &worldInvTrans);
		effect.SetVector(effect.getPerObject()->eyePosW, &eyePosW);
		effect.SetVariable(effect.getPerObject()->mat, reinterpret_cast<void *>(&obj->GetMat()), sizeof(obj->GetMat()));

		for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
		{
			technique->GetPassByIndex(pass)->Apply(0, context);

			context->DrawIndexed(obj->GetNumIndices(), 0, 0);
		}
	}
}
