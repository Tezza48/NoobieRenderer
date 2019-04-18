#include <d3d11_4.h>
#include <dxgi1_6.h>
#include "NoobieD3D.h"
#include <thread>
#include <windowsx.h>
#include "Utilities.h"
#include <string>

using namespace Noobie;

NoobieD3D * NoobieD3D::instance = nullptr;

NoobieD3D::NoobieD3D(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	: windowTitle(windowTitle), windowWidth(windowWidth), windowHeight(windowHeight)
{
	if (!instance)
	{
		instance = this;
	}

	depthStencilView = nullptr;
	renderTargetView = nullptr;
	swapChain = nullptr;
	context = nullptr;
	device = nullptr;
}

NoobieD3D::~NoobieD3D()
{
	SafeRelease(depthStencilView);
	SafeRelease(renderTargetView);
	SafeRelease(swapChain);
	SafeRelease(context);
	SafeRelease(device);

	DestroyWindow(windowHandle);

#if DEBUG || _DEBUG
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	SafeRelease(debug);
#endif // DEBUG || _DEBUG
}

bool NoobieD3D::Init()
{
	// Init Window
	HMODULE instance = GetModuleHandle(NULL);

	const wchar_t classname[] = L"WindowClass";
	// create the window
	WNDCLASS windowClass;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = ::MainWindowProc;// the global one
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = instance;
	windowClass.hIcon = LoadIcon(instance, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = classname;

	if (!RegisterClass(&windowClass))
	{
		printf("Error: Window failed to register.\n");
		return false;
	}

	windowHandle = CreateWindow(classname, windowTitle.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		windowWidth, windowHeight, 0, 0, instance, 0);

	if (windowHandle == 0)
	{
		printf("Error: CreateWindow Failed");
		return false;
	}

	ShowWindow(windowHandle, SW_SHOW);
	UpdateWindow(windowHandle);

	// Init D3D
	UINT createDeviceFlags = 0;
#if DEBUG || _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = windowWidth;
	sd.BufferDesc.Height = windowHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = windowHandle;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	sd.Flags = NULL;

	IDXGISwapChain * sc0;

	D3D_FEATURE_LEVEL featureLevel;

	D3D_CALL(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		createDeviceFlags, NULL, NULL, D3D11_SDK_VERSION, &sd, &sc0, 
		&device, &featureLevel, &context));
	sc0->QueryInterface(IID_PPV_ARGS(&swapChain));

	SafeRelease(sc0);

	const char devName[] = "Device";
	D3D_CALL(device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(devName) - 1, devName));
	const char ctxName[] = "Context";
	D3D_CALL(context->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(ctxName) - 1, ctxName));
	const char swapChainName[] = "SwapChain";
	D3D_CALL(swapChain->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(swapChainName) - 1, swapChainName));



	OnResize();

#if DEBUG || _DEBUG
	// 
	HRESULT hr = device->QueryInterface(IID_PPV_ARGS(&debug));
	if (FAILED(hr)) return false;
#endif

	return true;
}

void NoobieD3D::Run()
{
	Start();
	isRunning = true;
	OnResize();

	lastTime = high_resolution_clock::now();
	std::queue<float> frameTimeQueue;

	auto QueueAverage = [](std::queue<float> &queue, unsigned int max) -> float {
		while (max < queue.size())// reduce size to max
			queue.pop();
		float counter = 0;
		for (size_t i = 0; i < queue.size(); i++)
			counter += queue._Get_container()[i];
		return counter / queue.size();
	};

	bool done = false;
	while (isRunning)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			auto newTime = high_resolution_clock::now();
			duration<float> frameDuration = newTime - lastTime;
			lastTime = newTime;

			frameTimeQueue.emplace(frameDuration.count());

			float avgFrameTime = QueueAverage(frameTimeQueue, 1000);

			Update(frameDuration.count());
			Draw(frameDuration.count());

			std::wstring title = L"Frame Time : ";
			title += std::to_wstring(avgFrameTime);
			title += L"ms\t\t FPS: ";
			title += std::to_wstring((int)(1.0f / avgFrameTime));

			SetWindowText(windowHandle, title.c_str());

			DXGI_PRESENT_PARAMETERS pp;
			pp.DirtyRectsCount = 0;
			pp.pDirtyRects = NULL;
			pp.pScrollRect = NULL;
			pp.pScrollOffset = NULL;
			
			// Present
			D3D_CALL(swapChain->Present(doVsync ? 1 : 0, 0));

			SafeRelease(renderTargetView);
			// Re Bind new backbuffer
			ID3D11Texture2D * backBuffer;
			D3D_CALL(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
			D3D_CALL(device->CreateRenderTargetView(backBuffer, 0, &renderTargetView));
			SafeRelease(backBuffer);

			const char rtvName[] = "rtv";
			D3D_CALL(renderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(rtvName) - 1, rtvName));

			context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

			hasResized = false;
			input.Update();
		}
	}
}

void NoobieD3D::OnResize()
{
	// Release the old views
	SafeRelease(renderTargetView);
	SafeRelease(depthStencilView);

	// Re create the swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	swapChain->GetDesc(&sd);

	swapChain->ResizeBuffers(2, windowWidth, windowHeight, sd.BufferDesc.Format, 0);

	//SafeRelease(renderTargetView);

	ID3D11Texture2D * backBuffer;
	D3D_CALL(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	D3D_CALL(device->CreateRenderTargetView(backBuffer, 0, &renderTargetView));
	SafeRelease(backBuffer);

	const char rtvName[] = "RTV";
	D3D_CALL(renderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(rtvName) - 1, rtvName));

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width = windowWidth;
	dsd.Height = windowHeight;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count = 1;
	dsd.SampleDesc.Quality = 0;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;

	ID3D11Texture2D * depthStencilBuffer;
	D3D_CALL(device->CreateTexture2D(&dsd, 0, &depthStencilBuffer));
	D3D_CALL(device->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView));
	SafeRelease(depthStencilBuffer);

	const char dsvName[] = "DSV";
	D3D_CALL(renderTargetView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(dsvName) - 1, dsvName));

	context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(windowWidth);
	vp.Height = static_cast<float>(windowHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	context->RSSetViewports(1, &vp);
	hasResized = true;
}

void NoobieD3D::ClearBuffers(const float color[4])
{
	context->ClearRenderTargetView(renderTargetView, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

LRESULT CALLBACK NoobieD3D::WindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam)
{
	auto normalize = [](float pos, float size) -> float { return pos / size - 0.5f; };
	msg.hwnd = window;
	msg.message = message;
	msg.wParam = wparam;
	msg.lParam = lparam;

	switch (message)
	{
	case WM_SIZE:
		windowWidth = LOWORD(lparam);
		windowHeight = HIWORD(lparam);
		if (isRunning)
		{
			OnResize();
		}
		return 0;
	case WM_KEYDOWN:
		if (((1 << 30) & lparam) == 0)
			input.WndProcKeyState(static_cast<Input::KB>(wparam), true);
		return 0;
	case WM_KEYUP:
		input.WndProcKeyState(static_cast<Input::KB>(wparam), false);
		return 0;
	case WM_MOUSEMOVE:
		input.WndProcMouseMoved(
			normalize(static_cast<float>(GET_X_LPARAM(lparam)), static_cast<float>(windowWidth)),
			normalize(static_cast<float>(GET_Y_LPARAM(lparam)), -static_cast<float>(windowHeight)));
		return 0;
	case WM_LBUTTONDOWN:
		input.WndProcMouseButton(0, true);
		return 0;								  
	case WM_RBUTTONDOWN:
		input.WndProcMouseButton(1, true);
		return 0;								  
	case WM_MBUTTONDOWN:
		input.WndProcMouseButton(2, true);
		return 0;								
	case WM_LBUTTONUP:
		input.WndProcMouseButton(0, false);
		return 0;								
	case WM_RBUTTONUP:
		input.WndProcMouseButton(1, false);
		return 0;								 
	case WM_MBUTTONUP:
		input.WndProcMouseButton(2, false);
		return 0;
	case WM_DESTROY:
		isRunning = false;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(window, message, wparam, lparam);
}

LRESULT CALLBACK Noobie::MainWindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam)
{
	return NoobieD3D::GetInstance()->WindowProc(window, message, wparam, lparam);
}