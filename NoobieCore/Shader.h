#pragma once
#include <D3D11.h>
#include <string>
#include <DirectXMath.h>
#include "Lights.h"
#include "Utilities.h"

struct CBPerObject
{
	DirectX::XMFLOAT4X4 worldViewProj;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 worldInverseTranspose;
	DirectX::XMFLOAT3 eyePosW;
	float pad;
};

struct CBPerFrame
{
	float effectAmount;
	float pad[3];
};

struct CBLightBuffer
{
	DirectionalLight dirLight;
};

class Shader
{
private:
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
	ID3D11InputLayout* inputLayout;
	ID3D11RasterizerState* rasterizerState;
	ID3D11SamplerState* samplerState;

	ID3D11Buffer* cbPerObject;
	ID3D11Buffer* cbPerFrame;

public:

	Shader();
	~Shader();

	void Init(ID3D11Device* device, std::wstring path);
	void Bind(ID3D11Device* device, ID3D11DeviceContext* context);
	void UploadCBPerObject(ID3D11DeviceContext* context, CBPerObject& buffer);
	void UploadCBPerFrame(ID3D11DeviceContext* context, CBPerFrame& buffer);

private:
	void CreateRasterizerState(ID3D11Device * device, D3D11_RASTERIZER_DESC &desc, ID3D11RasterizerState ** state) const;
	void CreateSamplerState(ID3D11Device * device, D3D11_SAMPLER_DESC &desc, ID3D11SamplerState ** state) const;
	template<class T> void CreateConstantBuffer(ID3D11Device * device, ID3D11Buffer ** buffer) const;
};

template<class T>
inline void Shader::CreateConstantBuffer(ID3D11Device* device, ID3D11Buffer** buffer) const
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(T);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	T initBuffer;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &initBuffer;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	D3D_CALL(device->CreateBuffer(&cbDesc, &initData, buffer));
}
