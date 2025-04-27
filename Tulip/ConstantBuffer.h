#pragma once
#include"PixelShader.h"

class ConstantBuffer : CanvasComponent
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> BUFFER;
public:
	ConstantBuffer() = default;
	ConstantBuffer(ID3D11Device* device, const size_t size)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = size;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		//CallOnDevice(c3d, &ID3D11Device::CreateBuffer, &desc, nullptr, &BUFFER);
		device->CreateBuffer(&desc, nullptr, &BUFFER);
	}
	auto GetBuffer() const
	{
		return BUFFER;
	}
};