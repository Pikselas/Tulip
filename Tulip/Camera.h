#pragma once
#include <DirectXMath.h>
class Camera
{
private:
	DirectX::XMVECTOR look_at = DirectX::XMVectorZero();
private:
	float r = 1.0f;
	float rot_x = 0.0f, rot_y = 0.0f;
	float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;
public:
	void SetZ(float z) 
	{ 
		r = z; 
	}
	void SetLookAt(float x, float y, float z)
	{
		look_at = DirectX::XMVectorSet(x, y, z, 1.0f);
	}
	void RotateOrientation(int x, int y)
	{
		rot_x = DirectX::XM_PI * x / 180.0f;
		rot_y = DirectX::XM_PI * y / 180.0f;
	}
	void RotatePosition(int x, int y, int z)
	{
		roll = DirectX::XM_PI * x / 180.0f;
		pitch = DirectX::XM_PI * y / 180.0f;
		yaw = DirectX::XM_PI * z / 180.0f;
	}
	void RotateY(float angle)
	{
		rot_y = angle;
	}
	float GetZ() const
	{
		return r;
	}
	DirectX::XMMATRIX GetTransformation() const
	{
		const auto p = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f,0.0f,-r,1.0f), DirectX::XMMatrixRotationRollPitchYaw(rot_x, -rot_y, 0.0f));
		return DirectX::XMMatrixLookAtLH(p, look_at, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * DirectX::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);
	}
};