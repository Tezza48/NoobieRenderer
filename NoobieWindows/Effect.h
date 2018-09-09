#pragma once
#include <d3dx11effect.h>
#include "Utilities.h"
#include "VertexBuffer.h"

using Microsoft::WRL::ComPtr;

typedef ComPtr<ID3DX11Effect> PtrEffect;
typedef ComPtr<ID3DX11EffectTechnique> PtrEffectTechnique;
typedef ComPtr<ID3DX11EffectMatrixVariable> PtrEffectMatVar;

class Effect
{
private:
	// cbuffer stuff
	struct CBPerObject
	{
		PtrEffectMatVar worldViewProj = NULL;
	};

	PtrEffect effect = NULL;
	PtrEffectTechnique currentTechnique = NULL;
	PtrInputLayout inputLayout = NULL;

	CBPerObject cbPerObject;

public:
	Effect();
	virtual ~Effect();

	void Init(PtrDevice device);
	void Bind(PtrContext context);

	//void Draw();

	// Getters
	PtrEffectTechnique GetCurrentTechnique() const { return currentTechnique; }
	CBPerObject & GetCBPerObject() { return cbPerObject; }

	// Setters
	void SetMatrix(PtrEffectMatVar targetMat, XMMATRIX * value);
};
