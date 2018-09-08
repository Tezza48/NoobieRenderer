#pragma once
#include <comdef.h>
#include <tchar.h>
#include <wrl.h>
#include <d3d11.h>


#define D3D_CALL(x) if(FAILED(x)){\
	_com_error err(x);\
	wprintf(L"Error: in file %s at line (%d), hr: %s", _T(__FILE__), __LINE__, err.ErrorMessage());\
	DebugBreak();\
	}

using Microsoft::WRL::ComPtr;

typedef ComPtr<ID3D11Device> PtrDevice;
typedef ComPtr<ID3D11DeviceContext> PtrContext;
typedef ComPtr<ID3D11Buffer> PtrBuffer;
typedef ComPtr<ID3D11InputLayout> PtrInputLayout;
typedef ComPtr<ID3D11RenderTargetView> PtrRenderTargetView;
typedef ComPtr<ID3D11DepthStencilView> PtrDepthStencilView;


typedef ComPtr<IDXGISwapChain> PtrSwapChain;
