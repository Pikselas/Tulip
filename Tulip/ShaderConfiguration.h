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
		std::vector<std::reference_wrapper<Shader>> shaders;
	public:
		ShaderConfiguration() = default;
	public:
		void AddShader(Shader& shader)
		{
			shaders.push_back(shader);
		}
	public:
		void BindToContext(ID3D11DeviceContext* context) const override
		{
			for (const auto& shader : shaders)
			{
				shader.get().Bind(context);
			}
		}
	};

	/*class NonTexturedShaderConfig : public ShaderConfiguration
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

	class AsteroidShaderConfig : public ShaderConfiguration
	{
	public:
		AsteroidShaderConfig(ID3D11Device* device)
		{
			auto p_dir = GetProgramDirectory();

			const std::array<InputElemDesc, 2> input_desc =
			{
				InputElemDesc{"POSITION" ,InputElemDesc::INPUT_FORMAT::FLOAT3 , 0},
				InputElemDesc{"COLOR" ,InputElemDesc::INPUT_FORMAT::UINT4 , 12}
			};

			v_shader = VertexShader{ device , p_dir / "TerrainVertexShader.cso", std::span<const InputElemDesc>(input_desc) };
			p_shader = PixelShader{ device , p_dir / "AsteroidPixelShader.cso" };
		}
		void SetConstantBuffer(ConstantBuffer buffer)
		{
			v_shader.SetConstBuffer(buffer.GetBuffer().Get());
		}
	};*/

}