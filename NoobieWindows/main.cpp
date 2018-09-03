#include <Windows.h>
#include <vector>
#include <list>
#include "Renderer.h"

#include <d3dcompiler.h>
#include <fstream>
#include <string>
#include <sstream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

using namespace DirectX;

//ID3D11Device * device;
//ID3D11DeviceContext * context;

struct CBPerObject
{
	//XMMATRIX world;
	XMMATRIX worldViewProj;
};

void LoadOBJ(Renderer &renderer, std::string filename, VertexBuffer & vBuffer, IndexBuffer & iBuffer);

int main(int argc, char ** argv)
{
	{
		Window window;
		if (!window.Init(L"Noobie", 800, 600))
			return 0;

		Renderer renderer;
		renderer.Init(window);

		// Load stanford bunny model
		VertexBuffer bunnyvb;
		IndexBuffer bunnyib;
		LoadOBJ(renderer, "res/model/bunny.obj", bunnyvb, bunnyib);

		//std::vector<Vertex> vertices = {
		//	{XMFLOAT3(-0.5f, -0.5f, 0.0f)},
		//	{XMFLOAT3(0.5f, -0.5f, 0.0f)},
		//	{XMFLOAT3(0.0f, 0.45f, 0.0f)}
		//};

		//std::vector<unsigned int> indices = {
		//	0, 1, 2
		//};

		ID3D11RasterizerState * rs;

		D3D11_RASTERIZER_DESC rsd = {};
		rsd.FillMode = D3D11_FILL_SOLID;
		rsd.CullMode = D3D11_CULL_NONE;
		rsd.FrontCounterClockwise = false;
		rsd.DepthClipEnable = true;

		renderer.GetDevice()->CreateRasterizerState(&rsd, &rs);

		// input layout for vertex
		D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }/*,
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NOIRMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }*/
		};

		ID3DBlob * vsCode;
		ID3DBlob * psCode;
		//D3D_CALL(D3DReadFileToBlob(L"ColorVertex.cso", &vsCode));
		//D3D_CALL(D3DReadFileToBlob(L"ColorPixel.cso", &psCode));
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
		CBPerObject perObject{wvp};

		while (!window.ShouldWindowClose())
		{
			renderer.ClearBackBuffer(DirectX::Colors::CornflowerBlue);

			// Update Code

			// Draw Code

			renderer.GetContext()->RSSetState(rs);
			// Tell the gpu the layout to use
			renderer.GetContext()->IASetInputLayout(inputLayout);

			renderer.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Set the shaders
			renderer.GetContext()->VSSetShader(vertShader, NULL, 0);
			renderer.GetContext()->PSSetShader(pixelShader, NULL, 0);


			// Draw the index Buffer
			bunnyvb.Bind(renderer);
			bunnyib.Bind(renderer);
		
			renderer.GetContext()->DrawIndexed(bunnyib.GetNumIndices(), 0, 0);

			//renderer.GetContext()->Draw(3, 0);

			renderer.SwapBuffers();
		}

		inputLayout->Release();
		inputLayout = nullptr;
		pixelShader->Release();
		pixelShader = nullptr;
		vertShader->Release();
		vertShader = nullptr;
		rs->Release();
		rs = nullptr;
	}
	return 0;
}

void LoadOBJ(Renderer &renderer, std::string filename, VertexBuffer & vBuffer, IndexBuffer & iBuffer)
{
	std::ifstream file(filename, std::fstream::in);
	if (file.is_open())
	{
		std::list<Vertex> vertices;
		std::list<unsigned int> indices;

		// Read the file line by line
		std::string line = "";
		while (std::getline(file, line))
		{
			std::istringstream in(line);
			char type;
			in >> type;
			switch (type)
			{
				case 'v':
					// it's a vertex
					XMFLOAT3 pos;
					in >> pos.x >> pos.y >> pos.z;
					vertices.emplace_back(Vertex{pos});
					break;
				case 'f':
					// it's an index
					unsigned int a, b, c;
					in >> a >> b >> c;
					indices.emplace_back(a);
					indices.emplace_back(b);
					indices.emplace_back(c);
				default:
					break;
			}
		}

		//std::vector<Vertex> vVector();
		//std::vector<unsigned int> iVector;

		vBuffer = VertexBuffer(renderer, std::vector<Vertex>(std::begin(vertices), std::end(vertices)));
		iBuffer = IndexBuffer(renderer, std::vector<unsigned int>(std::begin(indices), std::end(indices)));

		file.close();

	}
	else
	{
		//something has gone wrong and i need to remedy it
	}
}