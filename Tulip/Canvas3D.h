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
	PtrManager<ID3D11DepthStencilView> DepthStencilView;
	PtrManager<ID3D11Buffer> ConstBuffer;
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
	DirectX::XMMATRIX ObjectTransform;
private:
	const float Halfheight;
	const float Halfwidth;
private:
	void UpdateCbuff(ID3D11Buffer * CBuffer , DirectX::XMMATRIX transform_matrix) const;
public:
	Canvas3D();
public:
	std::pair<float, float> GetNormalizedWindowPos(int x, int y) const;
	void ClearCanvas() const;
	void SetPrimitiveTopology(const PrimitiveTopology primitive) const;
	void DrawObject(const Object& obj , const ::Camera& camera);
	void SetRenderTarget(RenderTarget::Target& target);
};