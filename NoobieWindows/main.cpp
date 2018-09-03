#include "Window.h"
#include "Renderer.h"

#include <d3dcompiler.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

using namespace DirectX;

//ID3D11Device * device;
//ID3D11DeviceContext * context;

struct CBPerObject
{
	XMMATRIX world;
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

		//device = renderer.GetDevice();
		//context = renderer.GetContext();
	
		//Vertex cubeVertices[8] = {
		//	{ XMFLOAT3(-0.5f, -0.5f, -0.5f), /*static_cast<XMFLOAT4>(Colors::Red),		XMFLOAT3(-0.57735f, -0.57735f, -0.57735f)	*/},
		//	{ XMFLOAT3( 0.5f, -0.5f, -0.5f), /*static_cast<XMFLOAT4>(Colors::Orange),	XMFLOAT3( 0.57735f, -0.57735f, -0.57735f)	*/},
		//	{ XMFLOAT3( 0.5f, -0.5f,  0.5f), /*static_cast<XMFLOAT4>(Colors::Yellow),	XMFLOAT3( 0.57735f, -0.57735f,  0.57735f)	*/},
		//	{ XMFLOAT3(-0.5f, -0.5f,  0.5f), /*static_cast<XMFLOAT4>(Colors::Green),	XMFLOAT3(-0.57735f, -0.57735f,  0.57735f)	*/},
		//	{ XMFLOAT3( 0.5f,  0.5f, -0.5f), /*static_cast<XMFLOAT4>(Colors::Blue),		XMFLOAT3( 0.57735f,  0.57735f, -0.57735f)	*/},
		//	{ XMFLOAT3(-0.5f,  0.5f, -0.5f), /*static_cast<XMFLOAT4>(Colors::Indigo),	XMFLOAT3(-0.57735f,  0.57735f, -0.57735f)	*/},
		//	{ XMFLOAT3(-0.5f,  0.5f,  0.5f), /*static_cast<XMFLOAT4>(Colors::Violet),	XMFLOAT3(-0.57735f,  0.57735f,  0.57735f)	*/},
		//	{ XMFLOAT3( 0.5f,  0.5f,  0.5f), /*static_cast<XMFLOAT4>(Colors::Pink),		XMFLOAT3( 0.57735f,  0.57735f,  0.57735f)	*/}
		//};

		//unsigned int cubeIndices[36]{
		//	0, 1, 2,	0, 2, 3,
		//	1, 4, 7,	1, 7, 2,
		//	4, 5, 6,	4, 6, 7,
		//	5, 0, 3,	5, 3, 6,
		//	3, 2, 7,	3, 7, 6,
		//	5, 4, 1,	5, 1, 0
		//	};

		Vertex vertices[3] = {
			{XMFLOAT3(-0.5f, 0.0f, -0.5f)},
			{XMFLOAT3(0.5f, 0.0f, -0.5f)},
			{XMFLOAT3(0.0f, 0.0f, 0.45f)}
		};

		unsigned int indices[3] = {
			0, 1, 2
		};

		// Initialisation code

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
		psCode->Release();

		VertexBuffer vb(renderer, vertices, 3);
		IndexBuffer ib(renderer, indices, 3);

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
			vb.Bind();
			ib.Bind();
		
			renderer.GetContext()->DrawIndexed(ib.GetNumIndices(), 0, 0);

			renderer.SwapBuffers();
		}


		inputLayout->Release();
		pixelShader->Release();
		vertShader->Release();
		rs->Release();
	}
	return 0;
}
