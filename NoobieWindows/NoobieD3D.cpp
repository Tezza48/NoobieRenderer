#include "NoobieD3D.h"

#include <stdio.h>

NoobieD3D * NoobieD3D::instance = nullptr;

NoobieD3D::NoobieD3D(wstring windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	: windowTitle(windowTitle), windowWidth(windowWidth), windowHeight(windowHeight)
{
	if (!instance)
	{
		instance = this;
	}
}

NoobieD3D::~NoobieD3D()
{
	depthStencilView->Release();
	renderTargetView->Release();
	swapChain->Release();
	context->Release();
	device->Release();
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

	// Data returned from CreateDevice
	D3D_FEATURE_LEVEL featureLevel;
	D3D_CALL(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		createDeviceFlags, 0, 0, D3D11_SDK_VERSION,
		&device, &featureLevel, &context));

	D3D_CALL(device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa4xQuality));
	assert(msaa4xQuality > 0);

	OnResize();

	return true;
}

void NoobieD3D::Run()
{
	Start();
	isRunning = true;
	while (isRunning)
	{
		if (PeekMessage(&lastMessage, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&lastMessage);
			DispatchMessage(&lastMessage);
		}
		else
		{
			Update();
			Draw();
			D3D_CALL(swapChain->Present(0, 0));
		}
	}
}

void NoobieD3D::OnResize()
{
	// Re create the swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = windowWidth;
	sd.BufferDesc.Height = windowHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = enableMsaa ? 4 : 1;
	sd.SampleDesc.Quality = enableMsaa ? msaa4xQuality - 1 : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = windowHandle;
	sd.Windowed = isWindowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = NULL;

	IDXGIDevice * dxgiDevice = nullptr;
	D3D_CALL(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter * adapter = nullptr;
	D3D_CALL(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter));

	IDXGIFactory * factory = nullptr;
	D3D_CALL(adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory));

	D3D_CALL(factory->CreateSwapChain(device, &sd, &swapChain));
	D3D_CALL(factory->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_ALT_ENTER));

	dxgiDevice->Release();
	adapter->Release();
	factory->Release();

	ID3D11Texture2D * backBuffer;
	D3D_CALL(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	D3D_CALL(device->CreateRenderTargetView(backBuffer, 0, &renderTargetView));
	backBuffer->Release();

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width = windowWidth;
	dsd.Height = windowHeight;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count = enableMsaa ? 4 : 1;
	dsd.SampleDesc.Quality = enableMsaa ? msaa4xQuality - 1 : 0;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;

	ID3D11Texture2D * depthStencilBuffer;
	D3D_CALL(device->CreateTexture2D(&dsd, 0, &depthStencilBuffer));
	D3D_CALL(device->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView));
	depthStencilBuffer->Release();

	context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(windowWidth);
	vp.Height = static_cast<float>(windowHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	context->RSSetViewports(1, &vp);
}

LRESULT CALLBACK NoobieD3D::WindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam)
{
	lastMessage.hwnd = window;
	lastMessage.message = message;
	lastMessage.wParam = wparam;
	lastMessage.lParam = lparam;

	switch (message)
	{
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE)
		{
			isRunning = false;
			DestroyWindow(window);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(window, message, wparam, lparam);
}

void NoobieD3D::ClearBuffers(const float color[4])
{
	context->ClearRenderTargetView(renderTargetView, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

LRESULT CALLBACK MainWindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam)
{
	return NoobieD3D::GetInstance()->WindowProc(window, message, wparam, lparam);
}