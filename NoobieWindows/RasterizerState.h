#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "Renderer.h"

class RasterizerState
{
private:
	D3D11_RASTERIZER_DESC description;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> state;

public:
	RasterizerState(Renderer * renderer, D3D11_RASTERIZER_DESC desc);
	~RasterizerState();

	void Bind(Renderer * renderer);

	ID3D11RasterizerState * GetState() const { return state.Get(); }
};

