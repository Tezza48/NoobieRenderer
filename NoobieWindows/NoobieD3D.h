#pragma once
#include <d3d11.h>
#include <stdio.h>
#include <comdef.h>
#include <string>
#include <tchar.h>
#include <memory>
#include "Window.h"

#define D3D_CALL(x) if(FAILED(x)){\
	_com_error err(x);\
	wprintf(L"Error: in file %s at line (%d), hr: %s", _T(__FILE__), __LINE__, err.ErrorMessage());\
	DebugBreak();}

using std::wstring;
using std::unique_ptr;

LRESULT CALLBACK MainWindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam);

class NoobieD3D
{
private:
	static NoobieD3D * instance;

protected:
	// window information
	HWND windowHandle = 0;
	int windowWidth;
	int windowHeight;
	wstring windowTitle = L"Noobie";
	bool isWindowed = true;
	MSG lastMessage;

	// D3D information
	ID3D11Device * device;
	ID3D11DeviceContext * context;

	IDXGISwapChain * swapChain;

	ID3D11RenderTargetView * renderTargetView;
	ID3D11DepthStencilView * depthStencilView;

	bool enableMsaa = true;
	UINT msaa4xQuality;

	// Game information
	bool isRunning;
public:
	NoobieD3D(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight);
	~NoobieD3D();
	
	bool Init();
	void Run();

	// Window Callback Member, redirected to via the singleton
	LRESULT CALLBACK WindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam);

	static NoobieD3D * GetInstance() { return instance; };
protected:

	void ClearBuffers(const float color[4]);

	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void OnResize();

};

