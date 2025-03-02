#pragma once

#include"Canvas3D.h"
#include"Image.h"
#include"CanvasComponent.h"

class PixelShader;

class Texture : CanvasComponent
{
	friend PixelShader;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> TEXTURE;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TEXTURE_VIEW;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SAMPLER;
private:
	const Canvas3D& cnvs;
private:
	size_t width;
	size_t height;
public:
	Texture(const Canvas3D& canvas, const Image& img) : cnvs(canvas), width(img.GetWidth()), height(img.GetHeight())
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		auto data = img.Raw();
		subresource_data.pSysMem = data;
		subresource_data.SysMemPitch = sizeof(*data) * width;

		subresource_data.SysMemSlicePitch = 0;
		CallOnDevice(canvas, &ID3D11Device::CreateTexture2D, &desc, &subresource_data, &TEXTURE);
		CallOnDevice(canvas, &ID3D11Device::CreateShaderResourceView, TEXTURE.Get(), nullptr, &TEXTURE_VIEW);
		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		CallOnDevice(canvas, &ID3D11Device::CreateSamplerState, &sampler_desc, &SAMPLER);
	}
	auto SetImage(const Image& img)
	{
		/*
		*  Only updates the portion of the texture that is covered by the image.
		*	(i.e if the image is smaller than the texture, the rest of the texture is not updated)
		*/
		D3D11_BOX dst_box = {};
		dst_box.left = 0;
		dst_box.top = 0;
		dst_box.front = 0;
		dst_box.right = img.GetWidth() < width ? img.GetWidth() : width;
		dst_box.bottom = img.GetHeight() < height ? img.GetHeight() : height;
		dst_box.back = 1;
		
		D3D11_TEXTURE2D_DESC desc = {};
		TEXTURE->GetDesc(&desc);
		D3D11_SUBRESOURCE_DATA subresource_data = {};
		auto data = img.Raw();
		subresource_data.pSysMem = data;
		subresource_data.SysMemPitch = img.GetWidth() * sizeof(*data);
		subresource_data.SysMemSlicePitch = 0;
		CallOnContext(cnvs, &ID3D11DeviceContext::UpdateSubresource, TEXTURE.Get(), 0, &dst_box, data, subresource_data.SysMemPitch, subresource_data.SysMemSlicePitch);
	}
	auto GetTextureView() const
	{
		return TEXTURE_VIEW;
	}
	auto GetSampler() const
	{
		return SAMPLER;
	}
};