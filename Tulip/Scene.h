#pragma once
#include "ShaderConfiguration.h"
#include "RenderSystem.h"

#include "Terrain.h"
#include "Cube.h"
#include "Camera.h"

class Scene
{
private:
	ConstantBuffer transformation_buffer;
	Shader::NonTexturedShaderConfig s_config;
	Shader::TerrainShaderConfig t_config;
private:
	Object space_shuttle;
	Object terrain_block_1;
	Object terrain_block_2;
private:
	Camera primary_camera;
public:
	Scene(auto& canvas) 
	 :
	s_config(canvas.Device.Get()),
	t_config(canvas.Device.Get()),
	transformation_buffer(canvas.Device.Get(), sizeof(DirectX::XMMATRIX))
	{
		space_shuttle = Cube<Canvas3D::VertexType>{ canvas };
		terrain_block_1 = terrain_block_2 = Terrain<Canvas3D::VertexType>{ canvas };

		space_shuttle.SetShaderConfig(s_config);
		terrain_block_1.SetShaderConfig(t_config);
		terrain_block_2.SetShaderConfig(t_config);
		
		space_shuttle.SetPosition(0.0f, 0.0f, 0.0f);

		terrain_block_1.SetPosition(-50.0f, 2.0f, 0.0f);
		terrain_block_2.SetPosition(-50.0f, 2.0f, 100.0f);

		terrain_block_1.RotatePositional((unsigned short)0, 0, 180);
		terrain_block_2.RotatePositional((unsigned short)0, 0, 180);

		s_config.SetConstantBuffer(transformation_buffer);
		t_config.SetConstantBuffer(transformation_buffer);

		primary_camera.SetZ(10.0f);
	}
private:
	void draw_obj(auto& canvas, Object& obj)
	{
		const auto matrix = DirectX::XMMatrixTranspose
		(
			obj.GetTansformMatrix() * primary_camera.GetTransformation() *
			DirectX::XMMatrixPerspectiveLH(1.0f, 600.0f / 800.0f, 1.0f, 100.0f)
		);
		canvas.UpdateCbuff(transformation_buffer.GetBuffer().Get(), matrix);
		obj.GetShaderConfig()->BindToContext(canvas.ImmediateContext.Get());
		canvas.DrawObject(obj, primary_camera);
	}
public:
	void SetMovementLocation(float x , float y)
	{
		auto angle_y = std::atan2(1.0f, x);
		auto angle_x = std::atan2(1.0f, y);

		auto angle_y_deg = (angle_y * 180.0f) / DirectX::XM_PI;
		auto angle_x_deg = (angle_x * 180.0f) / DirectX::XM_PI;

		primary_camera.SetZ(10.0f);
		//primary_camera.SetLookAt(x, y, primary_camera.GetZ());
		primary_camera.RotateOrientation(angle_x_deg - 90, angle_y_deg - 90);
		space_shuttle.RotateFixedPoint(0.0f, -angle_y, DirectX::XM_PIDIV2 - angle_x);
	}
	void Update()
	{
		auto pos = terrain_block_1.GetPosition();
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorSet(0.0f, 0.0f, -0.1f, 0.0f));
		if (DirectX::XMVectorGetZ(pos) < -100.0f)
			pos = DirectX::XMVectorSet(-50.0f, 2.0f, 100.0f, 0.0f);
		terrain_block_1.SetPosition(pos);

		pos = terrain_block_2.GetPosition();
		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorSet(0.0f, 0.0f, -0.1f, 0.0f));
		if (DirectX::XMVectorGetZ(pos) < -100.0f)
			pos = DirectX::XMVectorSet(-50.0f, 2.0f, 100.0f, 0.0f);
		terrain_block_2.SetPosition(pos);
	}
public:
	void Render(auto& canvas)
	{
		draw_obj(canvas , space_shuttle);
		draw_obj(canvas , terrain_block_1);
		draw_obj(canvas , terrain_block_2);
	}
};