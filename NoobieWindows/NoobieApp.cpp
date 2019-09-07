#include "NoobieApp.h"
#include <DirectXColors.h>
#include <math.h>
#include "Assets.h"
#include "NoobieCore/Renderable.h"
#include "ShapeGenerator.h"
#include <fstream>

NoobieApp::NoobieApp(std::string windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	: NoobieD3D(windowTitle, windowWidth, windowHeight)
{
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
}

void NoobieApp::Start()
{
	doVsync = true;

	camera = new FlyCamera(static_cast<float>(windowWidth) / windowHeight, 3.1416f / 2.0f, 0.01f, 1000.0f);
	//scene.push_back(camera);

	camera->SetPosition({ 0.0f, 2.0f, 0.0f });

	shader.Init(device, L"TexColor");

	//auto bunny = new Renderable("bunny", device, Game::Assets::LoadObj(Game::Assets::ModelEnum::BUNNY_OBJ, 10.0f)[0]);
	//scene.push_back(bunny);

	//auto planeGeom = Game::Assets::LoadObj("res/model/bunnyOnPlane/bunnyOnPlane.obj", 4.0f);

	//for (const auto& data : planeGeom)
	//{
	//	scene.push_back(new Renderable("floor", device, data));
	//}

	// TODO: Make Meshes have Mesh Parts.
	auto caveMeshData = Game::Assets::LoadObj("res/model/baked-corridor.obj", 1.0f);

	for (const auto& data : caveMeshData)
	{
		scene.push_back(new Renderable("cave", device, data));
	}

	XMStoreFloat4(&light.color, DirectX::Colors::Yellow);
	light.direction = { 0.0f, -1.0f, 0.0f, 1.0f};
}

bool NoobieApp::Update(float dt)
{
	if (Input::GetKey(GLFW_KEY_ESCAPE))
	{
		return false;
	}

	accTime +=dt;

	if (hasResized)
	{
		camera->SetAspectRatio(GetAspectRatio());
		camera->Resize(windowWidth, windowHeight);
	}

	camera->Update(dt);

	for (const auto obj : scene)
	{
		if (obj->GetDoUpdate())
			obj->Update(dt);
	}

	return true;
}

void NoobieApp::Draw(float dt)
{
	XMFLOAT4 clearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearBuffers(&clearColor.x);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto v = camera->GetView();
	auto p = camera->GetProj();
	auto vS = XMLoadFloat4x4(&v);
	auto pS = XMLoadFloat4x4(&p);

	for (const auto obj : scene)
	{
		auto renderable = dynamic_cast<Renderable *>(obj);
		if (renderable)
		{
			if (!renderable->GetIsVisible())
				continue;

			auto world = renderable->GetWorld();
			auto worldS = XMLoadFloat4x4(&world);
			XMMATRIX wvpS = worldS * vS * pS;

			auto wvpTransposedS = XMMatrixTranspose(wvpS);

			XMFLOAT4X4 wvpF4x4;
			XMStoreFloat4x4(&wvpF4x4, wvpTransposedS);

			XMFLOAT4X4 worldInvTrans;
			XMMATRIX worldInvTransS;
			{
				auto wS = worldS;
				wS.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

				XMVECTOR detS = XMMatrixDeterminant(wS);
				worldInvTransS = XMMatrixTranspose(XMMatrixInverse(&detS, wS));
			}

			XMStoreFloat4x4(&worldInvTrans, worldInvTransS);

			shader.UploadCBPerObject(context, 
				CBPerObject { wvpF4x4, world, worldInvTrans, camera->GetPosition() });
			shader.UploadCBLightBuffer(context, CBLightBuffer{ light });

			renderable->Bind(context);

			auto tex = renderable->GetTexture();

			if (tex) {
				context->PSSetShaderResources(0, 1, &tex);
			}/*
			else 
			{
				context->PSSetShaderResources(0, 1, nullptr);
			}*/

			shader.Bind(device, context);
			context->DrawIndexed(renderable->GetNumIndices(), 0, 0);
		}
	}
}
