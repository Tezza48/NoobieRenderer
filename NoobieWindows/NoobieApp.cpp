#include "NoobieApp.h"
#include <DirectXColors.h>
#include <math.h>
#include "Assets.h"
#include "NoobieCore/Renderable.h"

NoobieApp::NoobieApp(string windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	: NoobieD3D(windowTitle, windowWidth, windowHeight)
{
	camera = nullptr;
	angle = 0;
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

	camera = new Camera(static_cast<float>(windowWidth) / windowHeight, 3.1416f / 2.0f, 0.01f, 1000.0f);
	//scene.push_back(camera);

	effect.Init(device);
	effect.SetTechnique("Default");

	auto bunny = new Renderable(device, Assets::LoadObj(Assets::ModelEnum::BUNNY_OBJ, 10.0f));
	scene.push_back(bunny);
}

void NoobieApp::Update(float dt)
{
	accTime +=dt;

	if (hasResized)
	{
		camera->SetAspectRatio(GetAspectRatio());
	}

	if (Input::GetKey(GLFW_KEY_Q))
	{
		angle -= dt;
	}

	if (Input::GetKey(GLFW_KEY_E))
	{
		angle += dt;
	}

	camera->SetPosition({ static_cast<float>(cos(angle)) * 2.0f, 2.0f, static_cast<float>(sin(angle) * 2.0f) });
	camera->LookAt({ 0.0f, 0.5f, 0.0f });
	camera->Update(dt, input);

	for (const auto obj : scene)
	{
		if (obj->GetDoUpdate())
			obj->Update(dt, input);
	}
}

void NoobieApp::Draw(float dt)
{
	XMFLOAT4 ambient = XMFLOAT4(0.1f, 0.1f, 0.2f, 1.0f);
	XMFLOAT4 ambient2 = XMFLOAT4(0.1f, 0.15f, 0.25f, 1.0f);
	ClearBuffers(&ambient.x);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	effect.Bind(device, context);

	XMMATRIX vp = camera->GetView() * camera->GetProj();

	D3DX11_TECHNIQUE_DESC techDesc;

	auto technique = effect.GetCurrentTechnique();
	technique->GetDesc(&techDesc);

	auto ambientVector = XMLoadFloat4(&ambient2);
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
