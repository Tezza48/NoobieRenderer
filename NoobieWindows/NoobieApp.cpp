#include "NoobieApp.h"
#include <DirectXColors.h>
#include <math.h>
#include "Assets.h"
#include "NoobieCore/Renderable.h"
#include "ShapeGenerator.h"

NoobieApp::NoobieApp(string windowTitle, unsigned int windowWidth, unsigned int windowHeight)
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

	effect.Init(device);
	effect.SetTechnique("Default");

	auto planeGeom = Game::Assets::LoadObj("res/model/noisyFloor.obj", 1.0f);

	for (const auto& data : planeGeom)
	{
		scene.push_back(new Renderable("floor", device, data));
	}

	auto bunny = new Renderable("bunny", device, Game::Assets::LoadObj(Game::Assets::ModelEnum::BUNNY_OBJ, 10.0f)[0]);
	scene.push_back(bunny);

	//bunny->SetPosition({ 5.0f, 0.0f, 0.0f });

	//auto caveMeshData = Game::Assets::LoadObj("res/model/fake-sky-cavern.obj", 1.0f);

	//for (const auto& data : caveMeshData)
	//{
	//	scene.push_back(new Renderable("cave", device, data));
	//}

	//auto dreadnoughtData = Game::Assets::LoadObj("res/model/dreadnought.obj", 0.1f);

	//for (const auto& data : dreadnoughtData)
	//{
	//	auto newRenderable = new Renderable("dreadnought", device, data);
	//	scene.push_back(newRenderable);
	//	newRenderable->SetPosition({ 0.0f, 2.0f, 0.0f });
	//}
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

	XMFLOAT4 ambient = XMFLOAT4(0.1f, 0.1f, 0.2f, 1.0f);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	effect.Bind(device, context);

	XMMATRIX vp = camera->GetView() * camera->GetProj();

	D3DX11_TECHNIQUE_DESC techDesc;

	auto technique = effect.GetCurrentTechnique();
	technique->GetDesc(&techDesc);

	auto ambientVector = XMLoadFloat4(&ambient);
	effect.SetVector(effect.getPerFrame()->ambientLight, &ambientVector);

	effect.SetVariable(effect.getPerFrame()->time, &accTime, sizeof(float));

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

			auto tex = renderable->GetTexture();
			if (tex) {
				effect.getDiffuseMap()->SetResource(tex);
			}

			for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
			{
				technique->GetPassByIndex(pass)->Apply(0, context);

				context->DrawIndexed(renderable->GetNumIndices(), 0, 0);
			}
		}
	}
}
