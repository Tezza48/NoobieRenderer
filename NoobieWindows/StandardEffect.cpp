#include "StandardEffect.h"

StandardEffect::StandardEffect()
{
	perObject = new PerObject();
	perFrame = new PerFrame();
}


StandardEffect::~StandardEffect()
{
	delete perObject;
	perObject = nullptr;
	delete perFrame;
	perFrame = nullptr;

	SafeRelease(inputLayout);
	SafeRelease(currentTechnique);
	SafeRelease(effect);
}

void StandardEffect::Init(ID3D11Device * device)
{
	UINT flags1 = 0;
	ID3DBlob * errors;
#if DEBUG || _DEBUG
	flags1 |= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	D3DX11CompileEffectFromFile(L"standard.fx", NULL, NULL, flags1, NULL, device, &effect, &errors);
	
	printf(static_cast<char *>(errors->GetBufferPointer()));
	
	errors->Release();
	currentTechnique = effect->GetTechniqueByName("Default");
	perObject->worldViewProj = effect->GetVariableByName("worldViewProj")->AsMatrix();
	perObject->world = effect->GetVariableByName("world")->AsMatrix();
	perObject->worldInverseTranspose = effect->GetVariableByName("worldInverseTranspose")->AsMatrix();
	perObject->eyePosW = effect->GetVariableByName("eyePosW")->AsVector();

	perFrame->ambientLight = effect->GetVariableByName("ambientLight")->AsVector();
}

void StandardEffect::Bind(ID3D11Device * device, ID3D11DeviceContext * context)
{
	if (inputLayout == NULL)
	{
		D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		D3DX11_PASS_DESC passDesc;
		currentTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
		device->CreateInputLayout(desc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &inputLayout);
	}

	context->IASetInputLayout(inputLayout);
}

void StandardEffect::SetMatrix(ID3DX11EffectMatrixVariable * targetMat, XMMATRIX * value)
{
	targetMat->SetMatrix(reinterpret_cast<float *>(value));
}

void StandardEffect::SetVector(ID3DX11EffectVectorVariable * targetVec, XMVECTOR * value)
{
	targetVec->SetFloatVector(reinterpret_cast<float *>(value));
}
