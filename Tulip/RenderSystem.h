#pragma once

#include "Target.h"
#include "ShaderConfiguration.h"


class RenderSystem
{
	friend class RenderSystemFactory;
private:
	std::vector<std::unique_ptr<RenderComponent>> render_components;
public:
	template<typename T>
	T* getComponent()
	{
		for (auto& c : render_components)
		{
			if (auto comp = dynamic_cast<T>(c.get()); comp != nullptr)
			{
				return comp;
			}
		}
	}
	void BindToContext(ID3D11DeviceContext* context) const
	{
		for (const auto& c : render_components)
		{
			c->BindToContext(context);
		}
	}
};

class RenderSystemFactory
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
public:
	RenderSystemFactory(Microsoft::WRL::ComPtr<ID3D11Device> device)
		:
		device(device)
	{}
	template<typename... T>
	RenderSystem CreateRenderSystem()
	{
		RenderSystem render_system;
		(render_system.render_components.emplace_back(std::make_unique<T>(device.Get())),...);
		return render_system;
	}
};