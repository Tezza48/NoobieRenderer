#include <d3d11_4.h>
#include <dxgi1_6.h>
#include "NoobieD3D.h"
#include <thread>
#include "Utilities.h"
#include <string>

using namespace Noobie;
using std::string;
using namespace std::chrono;

NoobieD3D * NoobieD3D::instance = nullptr;

NoobieD3D::NoobieD3D(string windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	: windowTitle(windowTitle), windowWidth(windowWidth), windowHeight(windowHeight)
{
#if DEBUG || _DEBUG
	debug = nullptr;
#endif
	if (!instance)
	{
		instance = this;
	}

	isRunning = false;

	window = nullptr;

	msg = { 0 };

	device = nullptr;
	context = nullptr;

	swapChain = nullptr;
	renderTargetView = nullptr;
	depthStencilView = nullptr;
}

NoobieD3D::~NoobieD3D()
{
	SafeRelease(depthStencilView);
	SafeRelease(renderTargetView);
	SafeRelease(swapChain);
	SafeRelease(context);
	SafeRelease(device);

	glfwTerminate();

#if DEBUG || _DEBUG
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	SafeRelease(debug);
#endif // DEBUG || _DEBUG
}

bool NoobieD3D::Init()
{
	// Init Window
	if (!glfwInit())
	{
		return false;
	}

	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return false;
	}

	glfwSetKeyCallback(window, Input::KeyCallback);

	glfwSetCursorPosCallback(window, Input::CursorPosCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	sd.OutputWindow = glfwGetWin32Window(window);
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
		isRunning = !glfwWindowShouldClose(window);
		glfwPollEvents();

		input.Update();

		auto newTime = high_resolution_clock::now();
		duration<float> frameDuration = newTime - lastTime;
		lastTime = newTime;

		frameTimeQueue.emplace(frameDuration.count());

		float avgFrameTime = QueueAverage(frameTimeQueue, 1000);

		isRunning = Update(frameDuration.count());
		Draw(frameDuration.count());

		string title = "Frame Time: ";
		title += std::to_string(avgFrameTime);
		title += "ms FPS: ";
		title += std::to_string(static_cast<int>(1.0f / avgFrameTime));

		glfwSetWindowTitle(window, title.c_str());

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

float Noobie::NoobieD3D::GetAspectRatio() const
{ 
	return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

int Noobie::NoobieD3D::GetWindowWidth() const
{
	return windowWidth;
}

int Noobie::NoobieD3D::GetWindowHeight() const
{
	return windowHeight;
}

int Noobie::GetWindowWidth()
{
	return NoobieD3D::GetInstance()->GetWindowWidth();
}

int Noobie::GetWindowHeight()
{
	return NoobieD3D::GetInstance()->GetWindowHeight();
}
