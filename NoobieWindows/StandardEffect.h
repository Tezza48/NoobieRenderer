#pragma once
#include <d3dx11effect.h>
#include "VertexBuffer.h"
#include <string>
#include "Utilities.h"

//struct DirLight
//{
//	XMFLOAT4 color;
//	XMFLOAT3 direction;
//	float pad;
//};

class StandardEffect
{
private:
	// cbuffer stuff
	struct CBPerObject
	{
		ID3DX11EffectMatrixVariable * worldViewProj = NULL;
		~CBPerObject()
		{
			SafeRelease(worldViewProj);
		}
	};

	//struct CBPerFrame
	//{
	//	DirLight dirLight;
	//};

	ID3DX11Effect * effect = NULL;
	ID3DX11EffectTechnique * currentTechnique = NULL;
	ID3D11InputLayout * inputLayout = NULL;

	CBPerObject * cbPerObject;
	//CBPerFrame cbPerFrame;

public:
	StandardEffect();
	virtual ~StandardEffect();

	void Init(ID3D11Device * device);
	void Bind(ID3D11Device * device, ID3D11DeviceContext * context);

	//void Draw();

	// Getters
	ID3DX11EffectTechnique * GetCurrentTechnique() const { return currentTechnique; }
	CBPerObject * GetCBPerObject() { return cbPerObject; }

	// Setters
	void SetMatrix(ID3DX11EffectMatrixVariable * targetMat, XMMATRIX * value);
	ID3DX11EffectTechnique * SetTechnique(std::string name) { currentTechnique = effect->GetTechniqueByName(name.c_str()); return currentTechnique; }
};

