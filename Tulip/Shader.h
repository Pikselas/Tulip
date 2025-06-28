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
	public:
		using ConstantBufferBinder = void (ID3D11DeviceContext::*)(UINT, UINT, ID3D11Buffer* const*);
	private:
		ConstantBufferBinder buffer_binder = nullptr;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> const_buffer;
	public:
		virtual ~Shader() = default;
	protected:
		void set_const_buffer_binder(ConstantBufferBinder binder)
		{
			buffer_binder = binder;
		}
	public:
		void SetConstBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> buffer)
		{
			const_buffer = buffer;
		}
		virtual void Bind(ID3D11DeviceContext* context) const
		{
			if(const_buffer)
				(context->*buffer_binder)(0u, 1u, const_buffer.GetAddressOf());
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
			
			set_const_buffer_binder(&ID3D11DeviceContext::VSSetConstantBuffers);
		}

		void Bind(ID3D11DeviceContext* context) const override
		{
			context->VSSetShader(shader.Get(), nullptr, 0u);
			context->IASetInputLayout(input_layout.Get());

			Shader::Bind(context);
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
			set_const_buffer_binder(&ID3D11DeviceContext::PSSetConstantBuffers);
		}
	public:
		void Bind(ID3D11DeviceContext* context) const override
		{
			context->PSSetShader(shader.Get() , nullptr , 0);
			Shader::Bind(context);
		}
	};
}