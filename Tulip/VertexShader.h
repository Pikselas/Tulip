#pragma once

#include<vector>
#include<optional>

#include"Canvas3D.h"
#include"CanvasComponent.h"

struct InputElemDesc
{
public:
	enum class INPUT_FORMAT
	{
		FLOAT2 = DXGI_FORMAT_R32G32_FLOAT,
		FLOAT3 = DXGI_FORMAT_R32G32B32_FLOAT,
		FLOAT4 = DXGI_FORMAT_R32G32B32A32_FLOAT,
		UINT4 = DXGI_FORMAT_R8G8B8A8_UNORM
	};
public:
	std::string SEMANTIC_NAME;
	INPUT_FORMAT FORMAT;
	unsigned int OFFSET;
};

class VertexShader : CanvasComponent
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> SHADER;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> INPUT_LAYOUT;
public:
	VertexShader(Canvas3D& c3d, const std::filesystem::path& cso_file, const std::span<InputElemDesc> descs)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
		D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
		std::vector<D3D11_INPUT_ELEMENT_DESC> iedescs;
		for (const auto& desc : descs)
		{
			iedescs.emplace_back(desc.SEMANTIC_NAME.c_str(), 0, static_cast<DXGI_FORMAT>(desc.FORMAT), 0, desc.OFFSET, D3D11_INPUT_PER_VERTEX_DATA, 0);
		}
		CallOnDevice(c3d,&ID3D11Device::CreateVertexShader,shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &SHADER);
		CallOnDevice(c3d, &ID3D11Device::CreateInputLayout, iedescs.data(), (UINT)iedescs.size(), shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), &INPUT_LAYOUT);
	}
	auto GetShader() const
	{
		return SHADER;
	}
	auto GetInputLayout() const
	{
		return INPUT_LAYOUT;
	}
};