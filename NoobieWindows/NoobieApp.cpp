#include "NoobieApp.h"
#include <DirectXColors.h>
#include "Content.h"

NoobieApp::~NoobieApp()
{
	fxWVP->Release();
}

void NoobieApp::Start()
{
	Content::LoadOBJ(device, "res/model/bunny.obj", bunny.vb, bunny.ib, 10.0f);

	// Load the Effect
	UINT flags1 = 0;
	#if DEBUG || _DEBUG
		flags1 |= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
	#endif

	D3DX11CompileEffectFromFile(L"effect.fx", NULL, NULL, flags1, NULL, device.Get(), &effect, nullptr);
	currentTech = effect->GetTechniqueByName("NormalTech");

	fxWVP = effect->GetVariableByName("worldViewProj")->AsMatrix();

	D3D11_INPUT_ELEMENT_DESC desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	currentTech->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(desc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, inputLayout.GetAddressOf());

	// Initialize Other data

	bunny.position = XMVectorSet(0.0f, -1.0f, 0.0f, 1.0f);
	XMVECTOR eyePos = XMVectorSet(0.0f, 0.0f, -3.0f, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	
	view = XMMatrixLookAtLH(eyePos, target, up);
	proj = XMMatrixPerspectiveFovLH(XM_PI / 4, GetAspectRatio(), 0.1f, 1000.0f);

}

void NoobieApp::Update(float dt)
{
	bunny.rotation += dt;
}

void NoobieApp::Draw(float dt)
{
	ClearBuffers(DirectX::Colors::CornflowerBlue);
	XMMATRIX wvp = XMMatrixTranslationFromVector(bunny.position) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), bunny.rotation) * view * proj;
	fxWVP->SetMatrix(reinterpret_cast<float *>(&wvp));

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(inputLayout.Get());

	bunny.vb.Bind(context);
	bunny.ib.Bind(context);

	D3DX11_TECHNIQUE_DESC techDesc;
	currentTech->GetDesc(&techDesc);

	for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
	{
		currentTech->GetPassByIndex(pass)->Apply(0, context.Get());

		context->DrawIndexed(bunny.ib.GetNumIndices(), 0, 0);
	}

}
