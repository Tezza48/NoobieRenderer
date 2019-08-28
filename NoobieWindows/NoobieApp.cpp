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

	camera->SetPosition({ 0.0f, 2.0f, 0.0f });

	effect.Init(device);
	effect.SetTechnique("Default");

	auto bunny = new Renderable(device, Assets::LoadObj(Assets::ModelEnum::BUNNY_OBJ, 10.0f));
	scene.push_back(bunny);
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
	}

	float speed = 2.5f * dt;

	XMFLOAT3 posInput(0, 0, 0);
	if (Input::GetKey(GLFW_KEY_A))
	{
		posInput.x -= speed;
	}

	if (Input::GetKey(GLFW_KEY_D))
	{
		posInput.x += speed;
	}

	if (Input::GetKey(GLFW_KEY_S))
	{
		posInput.z -= speed;
	}

	if (Input::GetKey(GLFW_KEY_W))
	{
		posInput.z += speed;
	}

	angle += (Input::GetDeltaMouse().x / windowWidth) * dt * 100.0f;
	pitch += (Input::GetDeltaMouse().y / windowHeight) * dt * 100.0f;

	pitch = min(max(pitch, -3.1f / 2.0f), 3.1f / 2.0f);


	XMFLOAT4 rotFloat4;
	auto newRot = XMQuaternionRotationRollPitchYaw(pitch, angle, 0.0f);
	XMStoreFloat4(&rotFloat4, newRot);

	camera->SetRotation(rotFloat4);

	XMVECTOR fwd = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), newRot) * posInput.z;
	XMVECTOR right = XMVector3Rotate(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), newRot) * posInput.x;

	auto delta = fwd + right;

	auto lastPos = XMLoadFloat3(&camera->GetPosition());

	auto newPos = lastPos + delta;

	XMFLOAT3 newPosf3;
	XMStoreFloat3(&newPosf3, newPos);

	camera->SetPosition(newPosf3);

	camera->Update(dt, input);

	for (const auto obj : scene)
	{
		if (obj->GetDoUpdate())
			obj->Update(dt, input);
	}

	return true;
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
