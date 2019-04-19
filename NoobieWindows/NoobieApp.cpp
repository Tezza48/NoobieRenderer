#include "NoobieApp.h"
#include <DirectXColors.h>
#include <math.h>
#include "DemoPlaneObject.h"
#include "DemoCylinderObject.h"
#include "Assets.h"
#include "Terrain.h"
#include "PlayerSpaceship.h"

NoobieApp::NoobieApp(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	: NoobieD3D(windowTitle, windowWidth, windowHeight)
{
	cone = nullptr;
	camera = nullptr;
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

	auto mapSize = 48U;

	float * heightmap = new float[mapSize * mapSize];

	for (size_t y = 0; y < mapSize; y++)
	{
		for (size_t x = 0; x < mapSize; x++)
		{
			heightmap[y * mapSize + x] = sin((float)x / mapSize * 24) + cos((float)y / mapSize * 24);
		}
	}

	auto terrain = new Terrain(device, heightmap, 48);
	scene.push_back(terrain);
	terrain->GetMat().diffuse = XMFLOAT4(0.2f, 0.8f, 0.1f, 1.0f);
	float specPower = 0.1f;
	terrain->GetMat().specular = XMFLOAT4(0.2f * specPower, 0.8f * specPower, 0.1f * specPower, 100.0f);

	auto bunny = new Renderable(device, Assets::LoadObj(Assets::ModelEnum::BUNNY_OBJ, 10.0f));
	scene.push_back(bunny);

	camera = new Camera(static_cast<float>(windowWidth) / windowHeight, 3.1416f / 2.0f, 0.01f, 1000.0f);

	cockpit = new PlayerSpaceship(device, camera);
	scene.push_back(cockpit);

	effect.Init(device);
	effect.SetTechnique("Default");
	
	scene.push_back(camera);
}

void NoobieApp::Update(float dt)
{
	if (input.GetKey(Input::KB_ESCAPE))
	{
		Quit();
		return;
	}
	accTime +=dt;

	if (hasResized)
	{
		camera->SetAspectRatio(GetAspectRatio());
	}

	for (const auto obj : scene)
	{
		if (obj->GetDoUpdate())
			obj->Update(dt, input);
	}

	if (input.GetKeyDown(input.KB_RETURN))
	{
		cockpit->SetIsVisible(!cockpit->GetIsVisible());
		std::puts("Pressed Return");
	}
}

void NoobieApp::Draw(float dt)
{
	XMFLOAT4 ambient = XMFLOAT4(0.1f, 0.1f, 0.2f, 1.0f);
	ClearBuffers(&ambient.x);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	effect.Bind(device, context);

	XMMATRIX vp = camera->GetView() * camera->GetProj();

	D3DX11_TECHNIQUE_DESC techDesc;

	auto technique = effect.GetCurrentTechnique();
	technique->GetDesc(&techDesc);

	auto ambientVector = XMLoadFloat4(&ambient);
	effect.SetVector(effect.getPerFrame()->ambientLight, &ambientVector);

	for (const auto obj : scene)
	{
		auto renderable = dynamic_cast<Renderable *>(obj);
		if (renderable)
		{
			if (!renderable->GetIsVisible())
				continue;

			auto world = renderable->GetWorld();
			XMMATRIX wvp = world * vp;
			XMMATRIX worldInvTrans;
			{
				auto w = world;
				w.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

				XMVECTOR det = XMMatrixDeterminant(w);
				worldInvTrans = XMMatrixTranspose(XMMatrixInverse(&det, w));
			}

			renderable->Bind(context);

			effect.SetMatrix(effect.getPerObject()->worldViewProj, &wvp);
			effect.SetMatrix(effect.getPerObject()->world, &world);
			effect.SetMatrix(effect.getPerObject()->worldInverseTranspose, &worldInvTrans);
			XMVECTOR eyePosW = XMLoadFloat3(&camera->GetPosition());
			effect.SetVector(effect.getPerObject()->eyePosW, &eyePosW);
			effect.SetVariable(effect.getPerObject()->mat, reinterpret_cast<void *>(&renderable->GetMat()), sizeof(renderable->GetMat()));

			for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
			{
				technique->GetPassByIndex(pass)->Apply(0, context);

				context->DrawIndexed(renderable->GetNumIndices(), 0, 0);
			}
		}
	}
}
