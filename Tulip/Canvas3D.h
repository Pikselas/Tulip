#pragma once
#include<span>
#include<wrl.h>
#include<vector>
#include<d3d11.h>
#include<d3dcompiler.h>
#include<DirectXMath.h>
#include<filesystem>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

#include"Window.h"
#include"Object.h"

#include"Target.h"
#include"Camera.h"

class Canvas3D
{
	friend class CanvasComponent;
	template<typename t>
	using PtrManager = Microsoft::WRL::ComPtr<t>;
public:
	PtrManager<ID3D11Device> Device;
	PtrManager<ID3D11DeviceContext> ImmediateContext;
public:
	struct VertexType
	{
		float x, y, z;
		unsigned char r, g, b, a;
	};
public:
	enum class PrimitiveTopology
	{
		Point	 = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		Line	 = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		Triangle = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	};
private:
	std::function<void()> DrawFunc = []() {};
private:
	const float Halfheight;
	const float Halfwidth;
public:
	template<typename T>
	void UpdateCbuff(ID3D11Buffer * CBuffer , const T& buffer_data ) const;
public:
	Canvas3D();
public:
	std::pair<float, float> GetNormalizedWindowPos(int x, int y) const;
	void SetPrimitiveTopology(const PrimitiveTopology primitive) const;
	void DrawObject(const Object& obj , const ::Camera& camera);
	void SetRenderTarget(RenderTarget::Target& target);
};

template<typename T>
inline void Canvas3D::UpdateCbuff(ID3D11Buffer* CBuffer, const T& buffer_data) const
{
	D3D11_MAPPED_SUBRESOURCE ms;
	ImmediateContext->Map(CBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
	std::memcpy(ms.pData, &buffer_data, sizeof(T));
	ImmediateContext->Unmap(CBuffer, 0u);
}
