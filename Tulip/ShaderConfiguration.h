#pragma once
#include "Shader.h"
#include "ConstantBuffer.h"
#include <array>
#include <span>
#include "RenderComponent.h"

namespace Shader
{
	class ShaderConfiguration : public RenderComponent
	{
	public:
		VertexShader v_shader;
		PixelShader p_shader;
	protected:
		static auto GetProgramDirectory()
		{
			char buffer[MAX_PATH];
			GetModuleFileName(nullptr, buffer, 100);
			std::filesystem::path path = buffer;
			return path.parent_path();
		}
	public:
		ShaderConfiguration() = default;
		ShaderConfiguration(VertexShader&& v_shader, PixelShader&& p_shader)
			:
			v_shader{ std::move(v_shader) },
			p_shader{ std::move(p_shader) }
		{
		}
	public:
		void BindToContext(ID3D11DeviceContext* context) const override
		{
			v_shader.Bind(context);
			p_shader.Bind(context);
		}
	};

	class NonTexturedShaderConfig : public ShaderConfiguration
	{
	public:
		NonTexturedShaderConfig(ID3D11Device* device) 
		{
			auto p_dir = GetProgramDirectory();
	
			const std::array<InputElemDesc, 2> input_desc =
			{
				InputElemDesc{"POSITION" ,InputElemDesc::INPUT_FORMAT::FLOAT3 , 0},
				InputElemDesc{"COLOR" ,InputElemDesc::INPUT_FORMAT::UINT4 , 12}
			};

			v_shader = VertexShader{ device , p_dir / "VertexShader.cso", std::span<const InputElemDesc>(input_desc) };
			p_shader = PixelShader{ device , p_dir / "PixelShader.cso" };
		}
		void SetConstantBuffer(ConstantBuffer buffer)
		{
			v_shader.SetConstBuffer(buffer.GetBuffer().Get());
		}
	};

	class TerrainShaderConfig : public ShaderConfiguration
	{
	public:
		TerrainShaderConfig(ID3D11Device* device)
		{
			auto p_dir = GetProgramDirectory();

			const std::array<InputElemDesc, 2> input_desc =
			{
				InputElemDesc{"POSITION" ,InputElemDesc::INPUT_FORMAT::FLOAT3 , 0},
				InputElemDesc{"COLOR" ,InputElemDesc::INPUT_FORMAT::UINT4 , 12}
			};

			v_shader = VertexShader{ device , p_dir / "TerrainVertexShader.cso", std::span<const InputElemDesc>(input_desc) };
			p_shader = PixelShader{ device , p_dir / "TerrainPixelShader.cso" };
		}
		void SetConstantBuffer(ConstantBuffer buffer)
		{
			v_shader.SetConstBuffer(buffer.GetBuffer().Get());
		}
	};
}