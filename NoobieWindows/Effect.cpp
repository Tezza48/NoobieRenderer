#include "Effect.h"



Effect::Effect()
{
}


Effect::~Effect()
{
}

void Effect::Init(PtrDevice device)
{
	UINT flags1 = 0;
#if DEBUG || _DEBUG
	flags1 |= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	D3DX11CompileEffectFromFile(L"effect.fx", NULL, NULL, flags1, NULL, device.Get(), effect.GetAddressOf(), nullptr);
	currentTechnique = effect->GetTechniqueByName("NormalTech");

	cbPerObject.worldViewProj = effect->GetVariableByName("worldViewProj")->AsMatrix();

	D3D11_INPUT_ELEMENT_DESC desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	currentTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(desc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, inputLayout.GetAddressOf());
}

void Effect::Bind(PtrContext context)
{
	context->IASetInputLayout(inputLayout.Get());
}

void Effect::SetMatrix(PtrEffectMatVar targetMat, XMMATRIX * value)
{
	targetMat->SetMatrix(reinterpret_cast<float *>(value));
}