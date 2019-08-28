#pragma once
#include <d3d11.h>
#include <dxgi1_2.h>
#include <stdio.h>
#include <string>
#include <memory>
#include "Utilities.h"
#include <chrono>
#include <queue>
#include "Input.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Noobie
{
	int GetWindowWidth();
	int GetWindowHeight();

	class NoobieD3D
	{
	protected:
		// window information
		GLFWwindow * window;

		int windowWidth;
		int windowHeight;

		std::string windowTitle = "Noobie";
		
		bool isWindowed = true;

		MSG msg;

		// D3D information
		ID3D11Device * device;
		ID3D11DeviceContext * context;

		IDXGISwapChain1 * swapChain;
		ID3D11RenderTargetView * renderTargetView;
		ID3D11DepthStencilView * depthStencilView;

		bool doVsync = true;
		
		// Game Stuff
		Input input;

		bool hasResized = false;;

	private:
#if DEBUG || _DEBUG
		ID3D11Debug * debug;
#endif
		static NoobieD3D * instance;

		bool isRunning;
		std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
	
	public:
		NoobieD3D(std::string windowTitle, unsigned int windowWidth, unsigned int windowHeight);
		virtual ~NoobieD3D();
	
		bool Init();
		void Run();

		static NoobieD3D * GetInstance() { return instance; };
		float GetAspectRatio() const;
		int GetWindowWidth() const;
		int GetWindowHeight() const;
	
	protected:
		void ClearBuffers(const float color[4]);

		virtual void Start() = 0;
		virtual bool Update(float dt) = 0;
		virtual void Draw(float dt) = 0;
		virtual void OnResize();

		void Quit() { isRunning = false; }
	};
}
