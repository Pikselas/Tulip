#pragma once
#include <d3d11.h>
class RenderComponent
{
public:
	virtual void BindToContext(ID3D11DeviceContext*) const = 0;
	virtual ~RenderComponent() = default;
};