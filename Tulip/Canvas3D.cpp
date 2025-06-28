#include"Canvas3D.h"

Canvas3D::Canvas3D() : Halfheight(600 / 2), Halfwidth(800 / 2)
{
	if (auto hrcode = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &Device, nullptr, &ImmediateContext); FAILED(hrcode))
	{
		throw hrcode;
	}

	//draws the vertices as a list of TRIANGLE
	ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;							// uses the alpha channel of the source pixel as the blend factor,
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;					// uses the inverse of the alpha channel of the source pixel as the blend factor
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;								// adds the source and destination blend factors
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;							// it is fully opaque
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;		// enables all the color channels for writing

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	Device->CreateBlendState(&blendDesc, &blendState);
	ImmediateContext->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

	//creating geometry shader
	char buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, 100);
	std::filesystem::path path = buffer;
	path = path.parent_path();
	path /= "TextureGS.cso";

	Microsoft::WRL::ComPtr<ID3D11GeometryShader> GeometryShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(path.c_str(), &pBlob);
	Device->CreateGeometryShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &GeometryShader);
	ImmediateContext->GSSetShader(GeometryShader.Get(), nullptr, 0u);
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
	
	/*const auto matrix = DirectX::XMMatrixTranspose(
		obj.GetTransformMatrix() * p_camera.GetTransformation() *
		DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f , 1.0f, 40.0f)
	);*/
	
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
