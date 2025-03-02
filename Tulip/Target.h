#pragma once
#include<d3d11.h>
#include<wrl.h>

namespace RenderTarget
{
	class Target
	{
	private:
		const unsigned int width;
		const unsigned int height;
	public:
		ID3D11Resource* render_surface = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
	protected:
		Target(unsigned int width, unsigned int height)
			:
			width(width),
			height(height)
		{
		}
	private:
		ID3D11RenderTargetView* GetTarget() const
		{
			return render_target_view.Get();
		}
	protected:
		void CreateTarget(ID3D11Device* device, ID3D11Resource* surface)
		{
			render_surface = surface;
			device->CreateRenderTargetView(surface, nullptr, &render_target_view);
		}
	public:
		virtual void RenderFrame() = 0;
	public:
		void Clear()
		{
			const float color[] = { 0.0f,0.0f,0.0f,1.0f };
			Microsoft::WRL::ComPtr<ID3D11Device> device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
			render_surface->GetDevice(&device);
			device->GetImmediateContext(&context);
			context->ClearRenderTargetView(render_target_view.Get(), color);
		}
		void CopyFrame(const Target& renderer)
		{
			Microsoft::WRL::ComPtr<ID3D11Device> device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
			render_surface->GetDevice(&device);
			device->GetImmediateContext(&context);

			context->CopyResource(render_surface, renderer.render_surface);
		}
		unsigned int getHeight() const
		{
			return height;
		}
		unsigned int getWidth() const
		{
			return width;
		}
	};
}