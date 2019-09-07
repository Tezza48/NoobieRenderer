#include "Shader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "Vertex.h"

#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

Shader::Shader() : inputLayout(nullptr)
{

}

Shader::~Shader()
{
	SafeRelease(vs);
	SafeRelease(ps);
	SafeRelease(inputLayout);
	SafeRelease(rasterizerState);
	SafeRelease(samplerState);
	SafeRelease(cbPerObject);
	SafeRelease(cbPerFrame);
}

void Shader::Init(ID3D11Device* device, std::wstring path)
{
	std::ifstream ifs;

	std::vector<char> vsBytecode;

	auto passPath = path + L"_VS_P0.cso";
	ifs.open(passPath, std::ios::binary);
	if (!ifs.is_open()) 
		std::wcout << L"Could not load shader " << passPath.c_str();
	else
	{
		std::vector<char> bytecode = std::vector<char>(
			std::istreambuf_iterator<char>(ifs), 
			std::istreambuf_iterator<char>());

		vsBytecode = bytecode;

		D3D_CALL(device->CreateVertexShader(bytecode.data(), bytecode.size(), NULL, &vs));
	}
	ifs.close();

	passPath = path + L"_PS_P0.cso";
	ifs.open(passPath, std::ios::binary);
	if (!ifs.is_open())
		std::wcout << L"Could not load shader " << passPath.c_str();
	else
	{
		std::vector<char> bytecode = std::vector<char>(
			std::istreambuf_iterator<char>(ifs),
			std::istreambuf_iterator<char>());

		D3D_CALL(device->CreatePixelShader(bytecode.data(), bytecode.size(), NULL, &ps));
	}
	ifs.close();

	D3D11_INPUT_ELEMENT_DESC desc[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D_CALL(device->CreateInputLayout(desc, 4, vsBytecode.data(), vsBytecode.size(), &inputLayout));

	CreateConstantBuffer<CBPerObject>(device, &cbPerObject);
	CreateConstantBuffer<float[4]>(device, &cbPerFrame);

	auto rasDesc = CD3D11_RASTERIZER_DESC(
		D3D11_FILL_SOLID, D3D11_CULL_BACK, false, 0, 0, 0, true, false, false, false);

	CreateRasterizerState(device, rasDesc, &rasterizerState);

	D3D11_SAMPLER_DESC samDesc;
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.MipLODBias = 0.0f;
	samDesc.MaxAnisotropy = 1;
	samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samDesc.BorderColor[0] = 0;
	samDesc.BorderColor[1] = 0;
	samDesc.BorderColor[2] = 0;
	samDesc.BorderColor[3] = 0;
	samDesc.MinLOD = 0;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	CreateSamplerState(device, samDesc, &samplerState);

}

void Shader::Bind(ID3D11Device* device, ID3D11DeviceContext* context)
{
	context->VSSetShader(vs, NULL, 0);
	context->PSSetShader(ps, NULL, 0);
	context->IASetInputLayout(inputLayout);
	context->VSSetConstantBuffers(0, 1, &cbPerObject);
	context->PSSetConstantBuffers(0, 1, &cbPerObject);
	context->VSSetConstantBuffers(1, 1, &cbPerFrame);
	context->RSSetState(rasterizerState);
	context->PSSetSamplers(0, 1, &samplerState);
}

void Shader::UploadCBPerObject(ID3D11DeviceContext * context, CBPerObject& buffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	D3D_CALL(context->Map(cbPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer));

	memcpy(mappedBuffer.pData, &buffer, sizeof(buffer));

	context->Unmap(cbPerObject, 0);
}

void Shader::UploadCBPerFrame(ID3D11DeviceContext* context, CBPerFrame& buffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	D3D_CALL(context->Map(cbPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer));

	memcpy(mappedBuffer.pData, &buffer, sizeof(buffer));

	context->Unmap(cbPerFrame, 0);
}

void Shader::CreateRasterizerState(ID3D11Device* device, D3D11_RASTERIZER_DESC& desc, ID3D11RasterizerState** state) const
{
	D3D_CALL(device->CreateRasterizerState(&desc, state));
}

void Shader::CreateSamplerState(ID3D11Device* device, D3D11_SAMPLER_DESC& desc, ID3D11SamplerState** state) const
{
	D3D_CALL(device->CreateSamplerState(&desc, state));
}
