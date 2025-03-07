#pragma once
#include <d3d11.h>
#include <wrl.h>

class DepthBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
public:
	DepthBuffer(ID3D11Device* device, unsigned int width, unsigned int height)
	{
		D3D11_DEPTH_STENCIL_DESC dsd = {};
		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;

		device->CreateDepthStencilState(&dsd, &depth_stencil_state);

		//context->OMSetDepthStencilState(ds.Get(), 1u);

		//Depth stencil needs a buffer to write to

		D3D11_TEXTURE2D_DESC td = {};
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1u;
		td.ArraySize = 1u;
		td.Format = DXGI_FORMAT_D32_FLOAT;
		td.SampleDesc.Count = 1u;
		td.SampleDesc.Quality = 0u;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
		device->CreateTexture2D(&td, nullptr, &tex);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0u;

		device->CreateDepthStencilView(tex.Get(), &dsvd, &depth_stencil_view);

	}
	void Bind(ID3D11DeviceContext* context) const
	{
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
		context->OMGetRenderTargets(1u, &rtv, nullptr);

		context->OMSetDepthStencilState(depth_stencil_state.Get(), 1u);
		context->OMSetRenderTargets(1u, rtv.GetAddressOf(), depth_stencil_view.Get());
	}
	void Clear(ID3D11DeviceContext* context) const
	{
		context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}
};