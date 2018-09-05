#include <Windows.h>
#include <vector>
#include <array>
#include "Renderer.h"

#include <d3dcompiler.h>
#include <fstream>
#include <string>
#include <sstream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RasterizerState.h"
#include "Content.h"

using namespace DirectX;

struct CBPerObject
{
	//XMMATRIX world;
	XMMATRIX worldViewProj;
};

int main(int argc, char ** argv)
{
	{
		Window window;
		if (!window.Init(L"Noobie", 800, 600))
			return 0;

		Renderer renderer;
		renderer.Init(window);

		// Load stanford bunny model
		VertexBuffer objVertBuffer;
		IndexBuffer objIndexBuffer;
		Content::LoadOBJ(&renderer, "res/model/bunny.obj", objVertBuffer, objIndexBuffer);

		std::vector<Vertex> positions;
		positions.reserve(4);
		positions.emplace_back(-0.5f, -0.5f, 0.0f);
		positions.emplace_back( 0.5f, -0.5f, 0.0f);
		positions.emplace_back( 0.5f,  0.5f, 0.0f);
		positions.emplace_back(-0.5f,  0.5f, 0.0f);

		VertexBuffer quadVB;
		quadVB.Init(&renderer, positions);

		std::vector<unsigned int> indices = {
			0, 1, 2,	0, 2, 3
		};

		IndexBuffer quadIB;
		quadIB.Init(&renderer, indices);

		D3D11_RASTERIZER_DESC rsd = {};
		rsd.FillMode = D3D11_FILL_SOLID;
		rsd.CullMode = D3D11_CULL_NONE;
		rsd.FrontCounterClockwise = false;
		rsd.DepthClipEnable = true;

		RasterizerState rs(&renderer, rsd);
		// input layout for vertex
		D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }/*,
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }*/
		};

		ID3DBlob * vsCode;
		ID3DBlob * psCode;
		D3D_CALL(D3DCompileFromFile(L"ColorVertex.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3D10_SHADER_ENABLE_STRICTNESS, 0, &vsCode, NULL));
		D3D_CALL(D3DCompileFromFile(L"ColorPixel.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3D10_SHADER_ENABLE_STRICTNESS, 0, &psCode, NULL));

		ID3D11VertexShader * vertShader;
		ID3D11PixelShader * pixelShader;
	
		D3D_CALL(renderer.GetDevice()->CreateVertexShader(vsCode->GetBufferPointer(), vsCode->GetBufferSize(), 0, &vertShader));
		D3D_CALL(renderer.GetDevice()->CreatePixelShader(psCode->GetBufferPointer(), psCode->GetBufferSize(), 0, &pixelShader));

		ID3D11InputLayout * inputLayout;
		D3D_CALL(renderer.GetDevice()->CreateInputLayout(desc, 1, vsCode->GetBufferPointer(), vsCode->GetBufferSize(), &inputLayout));

		vsCode->Release();
		vsCode = nullptr;
		psCode->Release();
		psCode = nullptr;

		XMVECTOR eyePos = XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
		XMVECTOR target = XMVectorZero();
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

		XMMATRIX world = XMMatrixIdentity();
		XMMATRIX view = XMMatrixLookAtLH(eyePos, target, up);
		XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PI / 4, window.GetAspectRatio(), 0.1f, 1000.0f);

		XMMATRIX wvp = world * view * proj;
		//CBPerObject perObject{ XMMatrixTranspose(wvp) };
		CBPerObject perObject{ wvp };

		ID3D11Buffer * perObjectConstantBuffer;

		D3D11_BUFFER_DESC cbd;
		ZeroMemory(&cbd, sizeof(D3D11_BUFFER_DESC));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = sizeof(CBPerObject);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA cbInitData;
		cbInitData.pSysMem = &perObject;
		cbInitData.SysMemPitch = 0;
		cbInitData.SysMemSlicePitch = 0;

		D3D_CALL(renderer.GetDevice()->CreateBuffer(&cbd, &cbInitData, &perObjectConstantBuffer));

		while (!window.ShouldWindowClose())
		{
			renderer.ClearBackBuffer(DirectX::Colors::CornflowerBlue);

			// Update Code

			// Draw Code

			rs.Bind(&renderer);
			// Tell the gpu the layout to use
			renderer.GetContext()->IASetInputLayout(inputLayout);


			renderer.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Set the shaders
			renderer.GetContext()->VSSetShader(vertShader, NULL, 0);
			renderer.GetContext()->VSSetConstantBuffers(0, 1, &perObjectConstantBuffer);
			renderer.GetContext()->PSSetShader(pixelShader, NULL, 0);


			// Draw the index Buffer
			objVertBuffer.Bind(&renderer);
			objIndexBuffer.Bind(&renderer);

			renderer.DrawIndexed(objIndexBuffer.GetNumIndices());

			quadVB.Bind(&renderer);
			quadIB.Bind(&renderer);
		
			//renderer.GetContext()->DrawIndexed(objIndexBuffer->GetNumIndices(), 0, 0);

			//renderer.GetContext()->Draw(3, 0);

			//renderer.DrawIndexed(quadIB.GetNumIndices());

			renderer.SwapBuffers();
		}

		inputLayout->Release();
		inputLayout = nullptr;
		pixelShader->Release();
		pixelShader = nullptr;
		vertShader->Release();
		vertShader = nullptr;
	}
	return 0;
}