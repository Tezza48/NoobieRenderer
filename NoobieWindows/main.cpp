#include <Windows.h>
#include <vector>
#include <array>
#include "Renderer.h"

#include <d3dcompiler.h>
#include <d3dx11Effect.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RasterizerState.h"
#include "Content.h"
//#include "ConstantBuffer.h"

#include "NoobieApp.h"


//https://github.com/Microsoft/FX11/wiki/Effects-11
//https://blogs.msdn.microsoft.com/chuckw/2013/08/20/living-without-d3dx/
//https://blogs.msdn.microsoft.com/chuckw/2012/10/23/effects-for-direct3d-11-update/



using namespace DirectX;

//struct CBPerObject
//{
//	//XMMATRIX world;
//	//XMMATRIX worldInverseTranspose;
//	XMMATRIX worldViewProj;
//};

//struct DirectionalLight
//{
//	XMFLOAT3 direction;
//	float pad0;
//	XMFLOAT4 diffuseColor;
//};

//struct LightBuffer
//{
//	DirectionalLight light;
//};



int main(int argc, char ** argv)
{
	Window window;
	if (!window.Init(L"Noobie", 800, 600))
		return 0;

	Renderer renderer;
	renderer.Init(window);

	// Load stanford bunny model
	VertexBuffer objVertBuffer;
	IndexBuffer objIndexBuffer;
	Content::LoadOBJ(&renderer, "res/model/bunny.obj", objVertBuffer, objIndexBuffer, 10.0f);

	std::vector<Vertex> positions(4);
	positions[0].position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	positions[0].color =	XMFLOAT4(1.0f, 1.0f, 0.01f, 1.0f);
	positions[0].normal =	XMFLOAT3(0.0f, 0.0f, -1.0f);

	positions[1].position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	positions[1].color =	XMFLOAT4(1.0f, 1.0f, 0.01f, 1.0f);
	positions[1].normal =	XMFLOAT3(0.0f, 0.0f, -1.0f);

	positions[2].position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	positions[2].color =	XMFLOAT4(1.0f, 1.0f, 0.01f, 1.0f);
	positions[2].normal =	XMFLOAT3(0.0f, 0.0f, -1.0f);

	positions[3].position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	positions[3].color =	XMFLOAT4(1.0f, 1.0f, 0.01f, 1.0f);
	positions[3].normal =	XMFLOAT3(0.0f, 0.0f, -1.0f);

	VertexBuffer quadVB;
	quadVB.Init(&renderer, positions);

	std::vector<unsigned int> indices = {
		0, 2, 1,	0, 3, 2
	};

	IndexBuffer quadIB;
	quadIB.Init(&renderer, indices);

	// Load Effect
	ID3DX11Effect * effect= nullptr;
	ID3DX11EffectTechnique * tech = nullptr;
	ID3D11InputLayout * inputLayout = nullptr;
	ID3DX11EffectMatrixVariable * fxWVP = nullptr;
	
	ID3D10Blob * fxErrors = nullptr;

	UINT flags1 = 0;
	#if DEBUG || _DEBUG
		flags1 |= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
	#endif

	D3DX11CompileEffectFromFile(L"effect.fx", NULL, NULL, flags1, NULL, renderer.GetDevice(), &effect, &fxErrors);
	tech = effect->GetTechniqueByName("NormalTech");

	fxWVP = effect->GetVariableByName("worldViewProj")->AsMatrix();

	D3D11_INPUT_ELEMENT_DESC desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	tech->GetPassByIndex(0)->GetDesc(&passDesc);
	renderer.GetDevice()->CreateInputLayout(desc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &inputLayout);

	fxErrors->Release();
	fxErrors = nullptr;
	// End Loading Effect

	XMVECTOR eyePos = XMVectorSet(0.0f, 0.0f, -3.0f, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	XMMATRIX worldBunny = XMMatrixTranslationFromVector(XMVectorSet(-0.0f, -1.0f, 0.0f, 1.0f));
	XMMATRIX worldQuad = XMMatrixTranslationFromVector(XMVectorSet(0.5f, 0.0f, 0.0f, 1.0f));
	XMMATRIX view = XMMatrixLookAtLH(eyePos, target, up);
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PI / 4, window.GetAspectRatio(), 0.1f, 1000.0f);

	XMMATRIX worldInverse = XMMatrixIdentity();// THIS IS NOT CORRECT, JUST FOR DEBUG
	XMMATRIX wvpBunny = worldBunny * view * proj;
	XMMATRIX wvpQuad = worldQuad * view * proj;

	float rotationy = 0;

	while (!window.ShouldWindowClose())
	{
		renderer.ClearBackBuffer(DirectX::Colors::CornflowerBlue);

		// Update Code

		worldBunny *= XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), 0.0001f);

		// Draw Code

		wvpBunny = worldBunny * view * proj;
		fxWVP->SetMatrix(reinterpret_cast<float *>(&wvpBunny));

		renderer.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderer.GetContext()->IASetInputLayout(inputLayout);
		objVertBuffer.Bind(&renderer);
		objIndexBuffer.Bind(&renderer);

		D3DX11_TECHNIQUE_DESC techDesc;
		tech->GetDesc(&techDesc);

		for (unsigned int pass = 0; pass < techDesc.Passes; ++pass)
		{
			tech->GetPassByIndex(pass)->Apply(0, renderer.GetContext());

			renderer.DrawIndexed(objIndexBuffer.GetNumIndices());
		}
		renderer.SwapBuffers();
	}

	effect->Release();
	effect = nullptr;

	return 0;
}
int amain()
{
	NoobieApp app(L"Noobie Window", 800, 600);

	if (!app.Init())
		return 0;

	app.Run();

	return 0;
}