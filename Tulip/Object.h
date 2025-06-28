#pragma once
#include<vector>
#include<span>
#include<d3d11.h>
#include<wrl.h>
#include<DirectXMath.h>
#include<functional>
#include "CanvasComponent.h"
#include "ShaderConfiguration.h"

class Object : CanvasComponent
{
	friend class Canvas3D;
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
protected:
	Shader::ShaderConfiguration* shader_config = nullptr;
protected:
	float FixedPointRotationX = 0.0f;
	float FixedPointRotationY = 0.0f;
	float FixedPointRotationZ = 0.0f;
protected:
	float PositionalRotateX = 0.0f;
	float PositionalRotateY = 0.0f;
	float PositionalRotateZ = 0.0f;
protected:
	float m_PositionX = 0.0f;
	float m_PositionY = 0.0f;
	float m_PositionZ = 0.0f;
protected:
	DirectX::XMMATRIX transformation = DirectX::XMMatrixIdentity();
protected:
	size_t m_IndexCount = 0;
	unsigned int stride = 0;
protected:
	auto GetVBuff() const noexcept { return m_VertexBuffer; }
	auto GetIBuff() const noexcept { return m_IndexBuffer; }
	auto GetIndexCount() const noexcept { return m_IndexCount; }
public:
	auto GetShaderConfig() const noexcept { return shader_config; }
public:
	auto GetTransformMatrix() const noexcept
	{
		return (DirectX::XMMatrixRotationRollPitchYaw(FixedPointRotationX, FixedPointRotationY, FixedPointRotationZ) *
			DirectX::XMMatrixTranslation(m_PositionX, m_PositionY, m_PositionZ) * DirectX::XMMatrixRotationRollPitchYaw(PositionalRotateX, PositionalRotateY, PositionalRotateZ)) * transformation;
	}
public:
	template<typename VertexType>
	void Set(auto& canvas, std::span<const VertexType> vertices, std::span<const unsigned int> indices)
	{
		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = sizeof(VertexType) * vertices.size();						// total array size
		bd.Usage = D3D11_USAGE_DEFAULT;												// how buffer data will be used (read/write protections for GPU/CPU)
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;									// What type of buffer would it be
		bd.CPUAccessFlags = 0u;														// we don't want any cpu access for now so setting it to 0 for now
		bd.MiscFlags = 0u;															// misscellinious flag for buffer configuration (we don't want it now either)
		bd.StructureByteStride = sizeof(VertexType);								// Size of every vertex in the array 

		//holds the data pointer that will be used in vertex buffer

		D3D11_SUBRESOURCE_DATA subd = { 0 };
		subd.pSysMem = vertices.data();											// pointer to array so that it can copy all the array data to the buffer

		CallOnDevice(canvas, &ID3D11Device::CreateBuffer, &bd, &subd, &m_VertexBuffer);
		//Device->CreateBuffer(&bd, &subd, &VBuffer);

		D3D11_BUFFER_DESC ibd = { 0 };
		ibd.ByteWidth = sizeof(decltype(indices)::element_type) * indices.size();
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.StructureByteStride = sizeof(decltype(indices)::element_type);

		D3D11_SUBRESOURCE_DATA isubd = { 0 };
		isubd.pSysMem = indices.data();

		CallOnDevice(canvas, &ID3D11Device::CreateBuffer, &ibd, &isubd, &m_IndexBuffer);
		//Device->CreateBuffer(&ibd, &isubd, &m_IndexBuffe);

		m_IndexCount = indices.size();
	}
public:
	std::function<void(Object&)> OnUpdate = nullptr;
public:
	void RotateFixedPoint(const float x, const float y, const float z) noexcept
	{
		FixedPointRotationX = x;
		FixedPointRotationY = y;
		FixedPointRotationZ = z;
	}
	void RotatePositional(const float x, const float y, const float z)
	{
		PositionalRotateX = x;
		PositionalRotateY = y;
		PositionalRotateZ = z;
	}
	void RotateFixedPoint(const short x, const short y, const short z) noexcept
	{
		FixedPointRotationX = x * DirectX::XM_PI / 180;
		FixedPointRotationY = y * DirectX::XM_PI / 180;
		FixedPointRotationZ = z * DirectX::XM_PI / 180;
	}
	void RotatePositional(const short x, const short y, const short z)
	{
		PositionalRotateX = x * DirectX::XM_PI / 180;
		PositionalRotateY = y * DirectX::XM_PI / 180;
		PositionalRotateZ = z * DirectX::XM_PI / 180;
	}
	void SetPosition(const float x, const float y, const float z) noexcept
	{
		m_PositionX = x;
		m_PositionY = y;
		m_PositionZ = z;
	}
	void SetPosition(DirectX::XMVECTOR pos) noexcept
	{
		m_PositionX = DirectX::XMVectorGetX(pos);
		m_PositionY = DirectX::XMVectorGetY(pos);
		m_PositionZ = DirectX::XMVectorGetZ(pos);
	}
	void SetTransformation(DirectX::XMMATRIX mat)
	{
		transformation = mat;
	}
	void SetShaderConfig(Shader::ShaderConfiguration& config) noexcept
	{
		shader_config = &config;
	}
	DirectX::XMVECTOR GetPosition() const noexcept
	{
		return DirectX::XMVectorSet(m_PositionX, m_PositionY, m_PositionZ, 0.0f);
	}
};