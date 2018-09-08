#pragma once
#include <d3d11.h>
#include "Renderer.h"
#include <array>

template<typename _Ty>
class ConstantBuffer
{
private:
	ID3D11Buffer * buffer;
	_Ty data;

	ConstantBuffer();
public:
	ConstantBuffer(Renderer * renderer, _Ty initialData)
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(_Ty);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA cbInitData;
		cbInitData.pSysMem = &initialData;
		cbInitData.SysMemPitch = 0;
		cbInitData.SysMemSlicePitch = 0;

		D3D_CALL(renderer->GetDevice()->CreateBuffer(&desc, &cbInitData, &buffer));
	}

	~ConstantBuffer()
	{
		buffer->Release();
	}


	inline void BindVS(Renderer * renderer)
	{
		renderer->GetContext()->VSSetConstantBuffers(0, 1, &buffer);
	}

	inline void BindPS(Renderer * renderer)
	{
		renderer->GetContext()->PSSetConstantBuffers(0, 1, &buffer);
	}

	
	inline void Upload(Renderer * renderer)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		D3D_CALL(renderer->GetContext()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource));

		resource.pData = &data;

		renderer->GetContext()->Unmap(buffer, 0);
	}

	void SetData(_Ty newData) {data = newData;}
	_Ty * Get() { return & data; }
};