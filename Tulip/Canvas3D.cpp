#include"Canvas3D.h"

void Canvas3D::UpdateCbuff(ID3D11Buffer* CBuffer,DirectX::XMMATRIX transform_matrix) const
{	
	D3D11_MAPPED_SUBRESOURCE ms;
	ImmediateContext->Map(CBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
	std::memcpy(ms.pData, &transform_matrix, sizeof(transform_matrix));
	ImmediateContext->Unmap(CBuffer, 0u);
}

Canvas3D::Canvas3D() : Halfheight(600 / 2), Halfwidth(800 / 2)
{
	if (auto hrcode = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &Device, nullptr, &ImmediateContext); FAILED(hrcode))
	{
		throw hrcode;
	}

	//draws the vertices as a list of TRIANGLE
	ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Canvas3D::SetPrimitiveTopology(const PrimitiveTopology primitive) const
{
	ImmediateContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitive));
}

void Canvas3D::DrawObject(const Object& obj , const ::Camera& p_camera)
{
	UINT offset = 0u;														    // displacement after which the actual data start (so 0 because no displacement is there)
	ImmediateContext->IASetVertexBuffers(0u, 1u, obj.GetVBuff().GetAddressOf(), &obj.stride, &offset);
	ImmediateContext->IASetIndexBuffer(obj.GetIBuff().Get(), DXGI_FORMAT_R32_UINT, 0u);
	//ImmediateContext->VSSetShader(obj.GetVShader().Get(), nullptr, 0u);
	//ImmediateContext->IASetInputLayout(obj.GetILayout().Get());
	//ImmediateContext->PSSetShader(obj.GetPShader().Get(), nullptr, 0u);
	const auto IndexSize = obj.m_IndexCount;
	DrawFunc = [this, IndexSize]() { ImmediateContext->DrawIndexed(IndexSize, 0u, 0u); };
	
	const auto matrix = DirectX::XMMatrixTranspose(
		obj.GetTansformMatrix() * p_camera.GetTransformation() *
		DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f , 1.0f, 40.0f)
	);
	
	//if (auto cbuffer = obj.GetCBuffer())
	//{
	//	//ImmediateContext->VSSetConstantBuffers(0u, 1u, cbuffer->GetAddressOf());
	//	//UpdateCbuff(cbuffer->Get(), matrix);
	//}
	//if (auto texture = obj.GetTexture())
	//{
	//	ImmediateContext->PSSetSamplers(0u, 1u, texture->first.GetAddressOf());
	//	ImmediateContext->PSSetShaderResources(0u, 1u, texture->second.GetAddressOf());
	//}
	DrawFunc();
}

void Canvas3D::SetRenderTarget(RenderTarget::Target& target)
{
	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = target.getWidth();
	vp.Height = target.getHeight();
	vp.MaxDepth = 1;				// maximum depth for z axis
	vp.MinDepth = 0;				// minimum depth for z axis
	ImmediateContext->RSSetViewports(1u, &vp);
	ImmediateContext->OMSetRenderTargets(1u, target.render_target_view.GetAddressOf(), nullptr);
}

std::pair<float, float> Canvas3D::GetNormalizedWindowPos(int x, int y) const
{
	return { (x / Halfwidth) - 1.0f,  -((y / Halfheight) - 1.0f) };
}
