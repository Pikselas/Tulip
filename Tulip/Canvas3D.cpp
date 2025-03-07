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

void Canvas3D::ClearCanvas() const
{
	float color[] = { 0.0f , 0.0f , 0.0f , 0.0f };
	ImmediateContext->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Canvas3D::SetPrimitiveTopology(const PrimitiveTopology primitive) const
{
	ImmediateContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(primitive));
}

void Canvas3D::DrawObject(const Object& obj)
{
	UINT offset = 0u;														    // displacement after which the actual data start (so 0 because no displacement is there)
	ImmediateContext->IASetVertexBuffers(0u, 1u, obj.GetVBuff().GetAddressOf(), &obj.stride, &offset);
	ImmediateContext->IASetIndexBuffer(obj.GetIBuff().Get(), DXGI_FORMAT_R32_UINT, 0u);
	ImmediateContext->VSSetShader(obj.GetVShader().Get(), nullptr, 0u);
	ImmediateContext->IASetInputLayout(obj.GetILayout().Get());
	ImmediateContext->PSSetShader(obj.GetPShader().Get(), nullptr, 0u);
	const auto IndexSize = obj.m_IndexCount;
	DrawFunc = [this, IndexSize]() { ImmediateContext->DrawIndexed(IndexSize, 0u, 0u); };
	ObjectTransform = obj.GetTansformMatrix();
	const auto matrix = DirectX::XMMatrixTranspose(
		ObjectTransform * camera.GetTransformMatrix() *
		DirectX::XMMatrixPerspectiveLH(2.0f, Halfwidth / Halfheight, 1.0f, 40.0f)
	);
	if (auto cbuffer = obj.GetCBuffer())
	{
		ImmediateContext->VSSetConstantBuffers(0u, 1u, cbuffer->GetAddressOf());
		UpdateCbuff(cbuffer->Get(), matrix);
	}
	if (auto texture = obj.GetTexture())
	{
		ImmediateContext->PSSetSamplers(0u, 1u, texture->first.GetAddressOf());
		ImmediateContext->PSSetShaderResources(0u, 1u, texture->second.GetAddressOf());
	}
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
	ImmediateContext->OMSetRenderTargets(1u, target.render_target_view.GetAddressOf(), DepthStencilView.Get());
}

std::pair<float, float> Canvas3D::GetNormalizedWindowPos(int x, int y) const
{
	return { (x / Halfwidth) - 1.0f,  -((y / Halfheight) - 1.0f) };
}

void Canvas3D::Camera::Transform()
{
	const auto Pos = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(pos_x,pos_y, pos_z, 1.0f),
		DirectX::XMMatrixRotationRollPitchYaw(rot_x, rot_y, 0.0f));
	transform_matrix = DirectX::XMMatrixLookAtLH(Pos, DirectX::XMVectorZero(), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw);
}

Canvas3D::Camera::Camera()
{
	Transform();
}

void Canvas3D::Camera::RotateOrientation(const int x, const int y)
{
	rot_x = DirectX::XM_PI * x / 180.0f;
	rot_y = DirectX::XM_PI * y / 180.0f;
	Transform();
}

void Canvas3D::Camera::RotatePosition(const int x, const int y, const int z)
{
	roll = DirectX::XM_PI * x / 180.0f;
	pitch = DirectX::XM_PI * y / 180.0f;
	yaw = DirectX::XM_PI * z / 180.0f;
	Transform();
}

void Canvas3D::Camera::SetPosition(const float x, const float y, const float z)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
	Transform();
}

DirectX::XMMATRIX Canvas3D::Camera::GetTransformMatrix() const
{
	return transform_matrix;
}
