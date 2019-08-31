#pragma once
#include <d3dx11effect.h>
#include "VertexBuffer.h"
#include <string>
#include "Utilities.h"
#include "Material.h"

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
	struct PerObject
	{
		ID3DX11EffectMatrixVariable * worldViewProj = NULL;
		ID3DX11EffectMatrixVariable * world = NULL;
		ID3DX11EffectMatrixVariable * worldInverseTranspose = NULL;
		ID3DX11EffectVectorVariable * eyePosW = NULL;
		ID3DX11EffectVariable * mat = NULL;
		~PerObject()
		{
			SafeRelease(worldViewProj);
			SafeRelease(world);
			SafeRelease(worldInverseTranspose);
			SafeRelease(eyePosW);
			SafeRelease(mat);
		}
	};

	struct PerFrame
	{
		ID3DX11EffectVectorVariable * ambientLight;
		ID3DX11EffectVariable * time;
		~PerFrame()
		{
			SafeRelease(ambientLight);
			SafeRelease(time);
		}
	};

	ID3DX11Effect * effect = NULL;
	ID3DX11EffectTechnique * currentTechnique = NULL;
	ID3D11InputLayout * inputLayout = NULL;

	PerObject * perObject;
	PerFrame * perFrame;

public:
	StandardEffect();
	virtual ~StandardEffect();

	void Init(ID3D11Device * device);
	void Bind(ID3D11Device * device, ID3D11DeviceContext * context);

	//void Draw();

	// Getters
	ID3DX11EffectTechnique * GetCurrentTechnique() const { return currentTechnique; }
	PerObject * getPerObject() { return perObject; }
	PerFrame * getPerFrame() { return perFrame; }

	// Setters
	void SetMatrix(ID3DX11EffectMatrixVariable * targetMat, XMMATRIX * value);
	void SetVector(ID3DX11EffectVectorVariable * targetVec, XMVECTOR * value);
	void SetVariable(ID3DX11EffectVariable * targetVar, void* value, size_t size);
	ID3DX11EffectTechnique * SetTechnique(std::string name) { currentTechnique = effect->GetTechniqueByName(name.c_str()); return currentTechnique; }
};

