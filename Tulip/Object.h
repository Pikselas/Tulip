#pragma once
#include<vector>
#include<span>
#include<d3d11.h>
#include<wrl.h>
#include<DirectXMath.h>
#include<functional>
#include"CanvasComponent.h"
class Object : CanvasComponent
{
	friend class Canvas3D;
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	std::optional<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffer;
	std::optional<std::pair<Microsoft::WRL::ComPtr<ID3D11SamplerState>, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> m_Texture;
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
	size_t m_IndexCount = 0;
	unsigned int stride = 0;
protected:
	auto GetVBuff() const noexcept { return m_VertexBuffer; }
	auto GetIBuff() const noexcept { return m_IndexBuffer; }
	auto GetVShader() const noexcept { return m_VertexShader; }
	auto GetILayout() const noexcept { return m_InputLayout; }
	auto GetPShader() const noexcept { return m_PixelShader; }
	auto GetCBuffer() const noexcept { return m_ConstantBuffer; }
	auto GetTexture() const noexcept { return m_Texture; }
public:
	auto GetTansformMatrix() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw(FixedPointRotationX, FixedPointRotationY, FixedPointRotationZ) *
			DirectX::XMMatrixTranslation(m_PositionX, m_PositionY, m_PositionZ) * DirectX::XMMatrixRotationRollPitchYaw(PositionalRotateX, PositionalRotateY, PositionalRotateZ);
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
		ibd.ByteWidth = sizeof(size_t) * indices.size();
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.StructureByteStride = sizeof(size_t);

		D3D11_SUBRESOURCE_DATA isubd = { 0 };
		isubd.pSysMem = indices.data();

		CallOnDevice(canvas, &ID3D11Device::CreateBuffer, &ibd, &isubd, &m_IndexBuffer);
		//Device->CreateBuffer(&ibd, &isubd, &m_IndexBuffe);

		m_IndexCount = indices.size();
	}
	void SetVShader(auto& vShader) noexcept
	{
		m_VertexShader = vShader.GetShader();
		m_InputLayout = vShader.GetInputLayout();
	}
	void SetPShader(auto& pShader) noexcept
	{
		m_PixelShader = pShader.GetShader();
	}
	void SetCBuffer(auto& cBuffer) noexcept
	{
		m_ConstantBuffer = cBuffer.GetBuffer();
	}
	void SetTexture(auto& texture) noexcept
	{
		m_Texture = std::make_pair(texture.GetSampler(), texture.GetTextureView());
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
	void RotateFixedPoint(const unsigned short x, const unsigned short y, const unsigned short z) noexcept
	{
		FixedPointRotationX = x * DirectX::XM_PI / 180;
		FixedPointRotationY = y * DirectX::XM_PI / 180;
		FixedPointRotationZ = z * DirectX::XM_PI / 180;
	}
	void RotatePositional(const unsigned short x, const unsigned short y, const unsigned short z)
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
};