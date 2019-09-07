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

	auto planeGeom = Game::Assets::LoadObj("res/model/bunnyOnPlane/bunnyOnPlane.obj", 4.0f);
	std::vector<Renderable*> planeScene;
	for (const auto& data : planeGeom)
	{
		auto meshPart = new Renderable("floor", device, data);
		planeScene.push_back(meshPart);
		scene.push_back(meshPart);
		meshPart->SetIsVisible(false);
	}

	// TODO: Make Meshes have Mesh Parts.
	auto coridorMeshData = Game::Assets::LoadObj("res/model/baked-corridor.obj", 1.0f);
	std::vector<Renderable*> coridorScene;
	for (const auto& data : coridorMeshData)
	{
		auto meshPart = new Renderable("cave", device, data);
		coridorScene.push_back(meshPart);
		scene.push_back(meshPart);
	}

	auto camTweenFunction = [=](float* vars)
	{
		camera->SetPosition({ vars[0], vars[1], vars[2] });
		camera->SetRotation({ vars[3], vars[4], vars[5], vars[6] });
	};

	float delay = 5.0f;
	
	timeline.AddAction(
		{
			0.0f + delay,
			[=]() {
				camera->userHasControl = false;
				camera->SetPosition({ 1.097155f, 2.455842f, -1.033959f });
				camera->SetRotation({ 0.014939f, -0.572719f, 0.010440f, 0.819549f });
				auto camPos = camera->GetPosition();
				auto camRot = camera->GetRotation();
				timeline.RunTweener(Tweener(2.0f, 
					camTweenFunction,
					camPos, camRot,
					{-16.509302f, 1.090131f, 1.141623f}, {-0.026838f, -0.736753f, -0.029293f, 0.674994f }));
			}
		}
	);

	timeline.AddAction(
		{
			2.0f + delay,
			[=]() {
				auto camPos = camera->GetPosition();
				auto camRot = camera->GetRotation();
				timeline.RunTweener(Tweener(2.0f, camTweenFunction,
					camPos, camRot,
					{ -29.621681f, 3.598183f, -0.939637f }, {0.034856f, -0.272233f, 0.009868f, 0.961549f }));
			}
		}
	);

	timeline.AddAction(
		{
			4.0f + delay,
			[=]() {
				auto camPos = camera->GetPosition();
				auto camRot = camera->GetRotation();
				timeline.RunTweener(Tweener(2.0f, camTweenFunction, camPos, camRot,
					{ -42.790699f, 3.845201f, -1.562578f }, { 0.033018f, 0.466925f, -0.017450f, 0.883508f }));
			}
		}
	);

	timeline.AddAction(
		{
			6.0f + delay,
			[=]() {
				auto camPos = camera->GetPosition();
				auto camRot = camera->GetRotation();
				timeline.RunTweener(Tweener(2.0f, camTweenFunction, camPos, camRot,
					{ -54.168045f, 3.027464f, 1.856231f }, { 0.005736f, 0.570295f, -0.003982f, 0.821410f }));
			}
		}
	);

	timeline.AddAction(
		{
			10.0f + delay,
			[=]() {
				//camera->userHasControl = true;
				for (const auto& mesh : coridorScene)
				{
					mesh->SetIsVisible(false);
				}
				for (const auto& mesh : planeScene)
				{
					mesh->SetIsVisible(true);
				}
				camera->SetPosition({ 0.143129f, 1.107142f, 2.782357f });
				camera->SetRotation({ -0.072274f, 0.892055f, -0.153985f, -0.418692f });
			}
		}
	);

	timeline.AddAction(
		{
			10.0f + delay,
			[=]() {
				auto camPos = camera->GetPosition();
				auto camRot = camera->GetRotation();
				timeline.RunTweener(Tweener(2.0f, camTweenFunction, camPos, camRot,
					{ -2.912320f, 0.498552f, -0.098433f }, { 0.033444f, 0.884835f, -0.064299f, 0.460232f }));
			}
		}
	);

	timeline.AddAction(
		{
			//12.0f,
			12.0f + delay,
			[=]() {
				auto camPos = camera->GetPosition();
				auto camRot = camera->GetRotation();
				timeline.RunTweener(Tweener(2.0f, camTweenFunction, camPos, camRot,
					{ 0.122512f, 0.361898f, -0.622905f }, { 0.012849f, -0.325504f, 0.004424f, 0.945443f }));
				timeline.RunTweener(Tweener(6.0f, [=](float* vars)
					{
						effectAmount = vars[0];
						printf("amount: %ff\n", effectAmount);
					}, { 0.0f }, { 0.2f }));
			}
		}
	);

	timeline.AddAction(
		{
			14.0f + delay,
			[=]() {
				auto camPos = camera->GetPosition();
				auto camRot = camera->GetRotation();
				timeline.RunTweener(Tweener(4.0f, camTweenFunction, camPos, camRot,
					{ -0.291910f, 1.603406f, 1.405403f }, { 0.050009f, -0.858826f, 0.502609f, 0.085453f }));
			}
		}
	);

	timeline.AddAction(
		{
			18.0f + delay,
			[=]() {
				auto camPos = camera->GetPosition();
				auto camRot = camera->GetRotation();
				timeline.RunTweener(Tweener(2.0f, camTweenFunction, camPos, camRot,
					{ -2.737120f, 5.589679f, -1.918860f }, { 0.576192f, 0.189950f, -0.139863f, 0.782534f }));
				timeline.RunTweener(Tweener(10.0f, [=](float* vars)
					{
						effectAmount = vars[0];
						printf("amount: %ff\n", effectAmount);
					}, { 0.2f }, { 0.8f }));
			}
		}
	);
}

bool NoobieApp::Update(float dt)
{
	if (Input::GetKey(GLFW_KEY_ESCAPE))
	{
		return false;
	}

	accTime +=dt;

	timeline.Tick(accTime);

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

	if (Input::GetKeyDown(GLFW_KEY_F) && camera->userHasControl)
	{
		auto camPos = camera->GetPosition();
		auto camRot = camera->GetRotation();
		std::printf("keyframe:\t{ %ff, %ff, %ff}, {%ff, %ff, %ff, %ff }\n", camPos.x, camPos.y, camPos.z, camRot.x, camRot.y, camRot.z, camRot.w);
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
			shader.UploadCBPerFrame(context, CBPerFrame{ effectAmount });

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
