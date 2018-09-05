#pragma once
#include <assert.h>
#include <d3d11.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <stdio.h>
#include <comdef.h>
#include <tchar.h>
#include "Window.h"

#define WFILE 

#define D3D_CALL(x) if(FAILED(x)){\
	_com_error err(x);\
	wprintf(L"Error: in file %s at line (%d), hr: %s", _T(__FILE__), __LINE__, err.ErrorMessage());\
	DebugBreak();}

class Renderer
{
private:

	ID3D11Device * device;
	ID3D11DeviceContext * context;

	IDXGISwapChain * swapChain;

	ID3D11Texture2D * depthStencilBuffer;

	ID3D11RenderTargetView * renderTargetView;
	ID3D11DepthStencilView * depthStencilView;

	bool enableMsaa = true;
	UINT msaa4xQuality;
private:
public:
	Renderer();
	~Renderer();

	void Init(const Window & window);
	void Resize(const Window & window);

	void ClearBackBuffer(const float color[4]);
	void SwapBuffers();

	void DrawIndexed(unsigned int count);

	ID3D11Device * GetDevice() const { return device; }
	ID3D11DeviceContext * GetContext() const { return context; }

	//void SetInputLayout(InputLayout layout);
	//void SetShader(Shader shader); // Set all shaders in shader object
	//void SetPrimitiveTopology(/* Enum for type */);
};

