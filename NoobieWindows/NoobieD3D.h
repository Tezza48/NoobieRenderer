#pragma once
#include <d3d11.h>
#include <stdio.h>
#include <string>
#include <memory>
#include "Utilities.h"
#include <chrono>
#include <queue>

using std::wstring;
using std::unique_ptr;

LRESULT CALLBACK MainWindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam);

using namespace std::chrono;

class NoobieD3D
{
private:
	static NoobieD3D * instance;

	bool isRunning;
	time_point<high_resolution_clock> lastTime;
protected:
	// window information
	HWND windowHandle = 0;
	int windowWidth;
	int windowHeight;
	wstring windowTitle = L"Noobie";
	bool isWindowed = true;
	MSG msg;

	// D3D information
	PtrDevice device;
	PtrContext context;

	PtrSwapChain swapChain;

	PtrRenderTargetView renderTargetView;
	PtrDepthStencilView depthStencilView;

	bool enableMsaa = true;
	UINT msaa4xQuality;
public:
	NoobieD3D(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight);
	virtual ~NoobieD3D();
	
	bool Init();
	void Run();

	// Window Callback Member, redirected to via the singleton
	LRESULT CALLBACK WindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam);

	static NoobieD3D * GetInstance() { return instance; };
	float GetAspectRatio() const { return static_cast<float>(windowWidth) / static_cast<float>(windowHeight); }
protected:

	void ClearBuffers(const float color[4]);

	virtual void Start() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw(float dt) = 0;
	virtual void OnResize();

};

