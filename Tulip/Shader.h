#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <span>
#include <filesystem>
#include<d3dcompiler.h>

#include "VertexShader.h"

namespace Shader
{
	class Shader
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> const_buffer;
		void (ID3D11DeviceContext::* set_const_buffer)(UINT, UINT, ID3D11Buffer* const*) = nullptr;
	public:
		virtual ~Shader() = default;
	public:
		virtual void SetConstBuffer(ID3D11Buffer* context) = 0;
		virtual void Bind(ID3D11DeviceContext* context) const
		{
			if(const_buffer)
				(context->*set_const_buffer)(0u, 1u, const_buffer.GetAddressOf());
		}
	};

	class VertexShader : public Shader
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	public:
		VertexShader() = default;
		VertexShader(ID3D11Device* device, const std::filesystem::path& cso_file, const std::span<const InputElemDesc> descs)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
			D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
			std::vector<D3D11_INPUT_ELEMENT_DESC> iedescs;
			for (const auto& desc : descs)
			{
				iedescs.emplace_back(desc.SEMANTIC_NAME.c_str(), 0, static_cast<DXGI_FORMAT>(desc.FORMAT), 0, desc.OFFSET, D3D11_INPUT_PER_VERTEX_DATA, 0);
			}
			//CallOnDevice(c3d,&ID3D11Device::CreateVertexShader,shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &SHADER);
			//CallOnDevice(c3d, &ID3D11Device::CreateInputLayout, iedescs.data(), (UINT)iedescs.size(), shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), &INPUT_LAYOUT);
			device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &shader);
			device->CreateInputLayout(iedescs.data(), (UINT)iedescs.size(), shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), &input_layout);
		}

		void Bind(ID3D11DeviceContext* context) const override
		{
			context->VSSetShader(shader.Get(), nullptr, 0u);
			context->IASetInputLayout(input_layout.Get());

			Shader::Bind(context);
		}

		void SetConstBuffer(ID3D11Buffer* buffer) override
		{
			const_buffer = buffer;
			set_const_buffer = &ID3D11DeviceContext::VSSetConstantBuffers;
		}
	};

	class PixelShader : public Shader
	{
		Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
	public:
		PixelShader() = default;
		PixelShader(ID3D11Device* device, const std::filesystem::path& cso_file)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> shader_buffer;
			D3DReadFileToBlob(cso_file.c_str(), &shader_buffer);
			//CallOnDevice(c3d, &ID3D11Device::CreatePixelShader, shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &SHADER);
			device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), nullptr, &shader);
		}
	public:
		void Bind(ID3D11DeviceContext* context) const override
		{
			context->PSSetShader(shader.Get() , nullptr , 0);
			Shader::Bind(context);
		}
		void SetConstBuffer(ID3D11Buffer* buffer) override
		{
			const_buffer = buffer;
			set_const_buffer = &ID3D11DeviceContext::PSSetConstantBuffers;
		}
	};
}