#include "Renderer.h"



void Renderer::ClearBackBuffer(const float color[4])
{
	assert(context);
	assert(swapChain);
	context->ClearRenderTargetView(renderTargetView, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::SwapBuffers()
{
	D3D_CALL(swapChain->Present(0, 0));
}

Renderer::Renderer()
{
	
}


Renderer::~Renderer()
{
	depthStencilBuffer->Release();
	depthStencilView->Release();
	renderTargetView->Release();

	swapChain->Release();

	context->Release();
	device->Release();
}

void Renderer::Init(const Window & window)
{
	UINT createDeviceFlags = 0;
#if DEBUG || _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Data returned from CreateDevice
	D3D_FEATURE_LEVEL featureLevel;

	D3D_CALL(D3D11CreateDevice(NULL,
			 D3D_DRIVER_TYPE_HARDWARE,
			 NULL,
			 createDeviceFlags,
			 0, 0,
			 D3D11_SDK_VERSION,
			 &device,
			 &featureLevel,
			 &context));

	D3D_CALL(device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa4xQuality));
	assert(msaa4xQuality > 0);

	Resize(window);
}

void Renderer::Resize(const Window & window)
{
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = window.GetWindowWidth();
	sd.BufferDesc.Height = window.GetWindowHeight();
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = enableMsaa ? 4 : 1;
	sd.SampleDesc.Quality = enableMsaa ? msaa4xQuality - 1 : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = window.GetHandle();
	sd.Windowed = window.GetIsWindowed();
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = NULL;

	IDXGIDevice * dxgiDevice = nullptr;
	D3D_CALL(device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter * adapter = nullptr;
	D3D_CALL(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter));

	IDXGIFactory * factory = nullptr;
	D3D_CALL(adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory));

	D3D_CALL(factory->CreateSwapChain(device, &sd, &swapChain));
	factory->MakeWindowAssociation(window.GetHandle(), DXGI_MWA_NO_ALT_ENTER);

	dxgiDevice->Release();
	adapter->Release();
	factory->Release();

	ID3D11Texture2D * backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	device->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	backBuffer->Release();

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width = window.GetWindowWidth();
	dsd.Height = window.GetWindowHeight();
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count = enableMsaa ? 4 : 1;
	dsd.SampleDesc.Quality = enableMsaa ? msaa4xQuality - 1 : 0;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;

	D3D_CALL(device->CreateTexture2D(&dsd, 0, &depthStencilBuffer));

	D3D_CALL(device->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView));

	context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(window.GetWindowWidth());
	vp.Height = static_cast<float>(window.GetWindowHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	context->RSSetViewports(1, &vp);
}
