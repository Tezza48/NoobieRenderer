#include "RasterizerState.h"

RasterizerState::RasterizerState(Renderer * renderer, D3D11_RASTERIZER_DESC desc) : description(desc)
{
	D3D_CALL(renderer->GetDevice()->CreateRasterizerState(&description, state.GetAddressOf()));
}


RasterizerState::~RasterizerState()
{
	
}

void RasterizerState::Bind(Renderer * renderer)
{
	renderer->GetContext()->RSSetState(state.Get());
}
