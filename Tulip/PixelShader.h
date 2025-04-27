#pragma once
#include"Canvas3D.h"
#include"CanvasComponent.h"

class PixelShader 
{
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> SHADER;
public:
	PixelShader(ID3D11Device* device, const std::filesystem::path& cso_file)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
		D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
		//CallOnDevice(c3d, &ID3D11Device::CreatePixelShader, shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &SHADER);
		device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &SHADER);
	}
	auto GetShader() const
	{
		return SHADER;
	}
};