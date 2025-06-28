#pragma once

#include "CommonSceneHelpers.h"

class Map
{
private:
	Shader::VertexShader vertex_shader;
private:
	Shader::PixelShader asteroid_shader;
	Shader::PixelShader terrain_shader;
private:
	ConstantBuffer transformation_buffer;
	Shader::ShaderConfiguration t_config;
	Shader::ShaderConfiguration a_config;
private:
	struct Asteroid
	{
		Object obj;
		float rotation_x;
		float rotation_y;
		float rotation_z;
	};
private:
	std::array<Asteroid, 200> asteroids;
private:
	Object terrain_block_1;
	Object terrain_block_2;
private:
	DirectX::XMVECTOR movement_dir = DirectX::XMVectorSet(0.0f, 0.0f, -0.05f, 0.0f);
	DirectX::XMVECTOR positon = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
public:
	Map(Canvas3D& canvas)
		: 
	vertex_shader(canvas.Device.Get(), GetProgramDirectory() / "VertexShader.cso", std::span<const InputElemDesc>{ inp_desc }),
	asteroid_shader(canvas.Device.Get(), GetProgramDirectory() / "AsteroidPixelShader.cso"),
	terrain_shader(canvas.Device.Get(), GetProgramDirectory() / "TerrainPixelShader.cso"),
	transformation_buffer(canvas.Device.Get(), sizeof(MatrixData))
	{
		vertex_shader.SetConstBuffer(transformation_buffer.GetBuffer());

		t_config.AddShader(vertex_shader);
		t_config.AddShader(terrain_shader);
		
		a_config.AddShader(vertex_shader);
		a_config.AddShader(asteroid_shader);

		terrain_block_1 = 
		terrain_block_2 = Terrain<Canvas3D::VertexType>{ canvas };
		Object sphere = Sphere<Canvas3D::VertexType>{ canvas };
		Object pyramid = Pyramid<Canvas3D::VertexType>{ canvas };

		//sphere.SetTransformation(DirectX::XMMatrixScaling(0.005f, 0.005f, 0.005f));

		terrain_block_1.SetShaderConfig(t_config);
		terrain_block_2.SetShaderConfig(t_config);

		sphere.SetShaderConfig(a_config);
		pyramid.SetShaderConfig(a_config);

		terrain_block_1.SetPosition(50.0f, -2.0f, 0.0f);
		terrain_block_2.SetPosition(50.0f, -2.0f, 100.0f);
		
		std::mt19937 gen{ std::random_device{}() };
		
		std::uniform_real_distribution<float> position_x{ -50.0, 50.0 };
		std::uniform_real_distribution<float> position_z{ 0.0, 100.0 };
		std::uniform_real_distribution<float> position_y{ 0.0, 10.0 };
		std::uniform_real_distribution<float> rotaion{ -1.0f, 1.0f };
		
		for (auto& ast : asteroids)
		{
			ast.obj = pyramid;
			ast.obj.SetPosition(position_x(gen), position_y(gen), position_z(gen));
			ast.rotation_x = rotaion(gen);
			ast.rotation_y = rotaion(gen);
			ast.rotation_z = rotaion(gen);
		}

		terrain_block_1.RotateFixedPoint((short)0, 0, 180);
		terrain_block_2.RotateFixedPoint((short)0, 0, 180);
	}
private:
	void draw_obj(Canvas3D& canvas, const Object& main_obj , const Camera& camera) const
	{
		auto obj = main_obj;

		obj.SetPosition(DirectX::XMVectorAdd(obj.GetPosition(), positon));
		auto transformation_buffer_data = CreateMatrixData(obj, camera);
		canvas.UpdateCbuff(transformation_buffer.GetBuffer().Get(), transformation_buffer_data);
		obj.GetShaderConfig()->BindToContext(canvas.ImmediateContext.Get());
		canvas.DrawObject(obj, camera);
	}
	void update_pos_on_map(Object& obj, float time, float respawn_dist)
	{
		auto pos = obj.GetPosition();
		pos = DirectX::XMVectorAdd(pos, movement_dir);
		if (DirectX::XMVectorGetZ(pos) < respawn_dist)
			pos = DirectX::XMVectorSet(DirectX::XMVectorGetX(pos), DirectX::XMVectorGetY(pos), 90.0f, 0.0f);
		obj.SetPosition(pos);
	}
public:
	void SetLocation(DirectX::XMVECTOR location)
	{
		positon = location;
	}
	DirectX::XMVECTOR GetLocation() const
	{
		return positon;
	}
	void SetMovementLocation(float x, float y)
	{
		movement_dir = DirectX::XMVectorSet(x, y, -0.5f, 0.0f);
	}
public:
	void Update(float time)
	{
		update_pos_on_map(terrain_block_1, time, -110.0f);
		update_pos_on_map(terrain_block_2, time, -110.0f);
		for (auto& ast : asteroids)
		{
			update_pos_on_map(ast.obj, time, -10.0f);
			ast.obj.RotateFixedPoint(ast.rotation_x * time, ast.rotation_y * time, ast.rotation_z * time);
		}
	}
	void Render(auto& canvas , const Camera& camera) const
	{
		draw_obj(canvas, terrain_block_1, camera);
		draw_obj(canvas, terrain_block_2, camera);
		for (auto& ast : asteroids)
		{
			draw_obj(canvas, ast.obj, camera);
		}
	}
};

class Scene
{
private:
	Shader::PixelShader pixel_shader;
	Shader::VertexShader vertex_shader;
private:
	ConstantBuffer transformation_buffer;
	Shader::ShaderConfiguration s_config;
private:
	Object space_shuttle;
public:
	Camera primary_camera;
private:
	DirectX::XMVECTOR movement_dir = DirectX::XMVectorSet(0.0f, 0.0f, -0.5f , 0.0f);
public:
	Map map;
	Scene(auto& canvas) 
	 :
	map(canvas),
	vertex_shader(canvas.Device.Get(), GetProgramDirectory() / "VertexShader.cso",std::span<const InputElemDesc>(inp_desc)),
	pixel_shader(canvas.Device.Get(), GetProgramDirectory() / "PixelShader.cso"),
	transformation_buffer(canvas.Device.Get(), sizeof(MatrixData))
	{
		vertex_shader.SetConstBuffer(transformation_buffer.GetBuffer());
		
		s_config.AddShader(vertex_shader);
		s_config.AddShader(pixel_shader);
		
		//space_shuttle = Cube<Canvas3D::VertexType>{ canvas };
		space_shuttle = ObjLoader<Canvas3D::VertexType>{ canvas , R"(C:\Users\Aritra Maji\Downloads\ej0vwvf0j8jk-lowpolycat\lowpolycat\cat.obj)" };
		//space_shuttle = ObjLoader<Canvas3D::VertexType>{ canvas , R"(C:\Users\Aritra Maji\Downloads\Hot_air_balloon_v1_L2.123c69a97f0e-9977-45dd-9570-457189ce2941\11809_Hot_air_balloon_l2.obj)" };
		space_shuttle.SetPosition(0.0f, 0.0f, 0.0f);
		space_shuttle.RotateFixedPoint((short)0, (short)-90, (short)0u);
		space_shuttle.SetTransformation(DirectX::XMMatrixScaling(0.005f , 0.005f , 0.005f));
		space_shuttle.SetShaderConfig(s_config);
		primary_camera.SetZ(10.0f);

		//s_config.SetConstantBuffer(transformation_buffer);
	}
private:
	void draw_obj(auto& canvas, Object& obj)
	{
		const auto matrix = CreateMatrixData(obj, primary_camera);
		canvas.UpdateCbuff(transformation_buffer.GetBuffer().Get(), matrix);
		obj.GetShaderConfig()->BindToContext(canvas.ImmediateContext.Get());
		canvas.DrawObject(obj, primary_camera);
	}
public:
	void SetMovementLocation(float x , float y)
	{
		//map.SetMovementLocation(-x, -y);
		movement_dir = DirectX::XMVector3Normalize(DirectX::XMVectorSet(-x, -y, 0.0f, 0.0f));
		movement_dir = DirectX::XMVectorScale(movement_dir, 0.3f);
		auto angle_y = std::atan2(1.0f, x);
		auto angle_x = std::atan2(1.0f, y);

		auto angle_y_deg = (angle_y * 180.0f) / DirectX::XM_PI;
		auto angle_x_deg = (angle_x * 180.0f) / DirectX::XM_PI;

		primary_camera.SetZ(10.0f);
		//primary_camera.SetLookAt(x, y, primary_camera.GetZ());
		primary_camera.RotateOrientation(angle_x_deg - 90, angle_y_deg - 90);
		//space_shuttle.RotateFixedPoint(0.0f, -angle_y, DirectX::XM_PIDIV2 - angle_x);
	}
	void Update(float time)
	{
		map.Update(time);
		auto pos = DirectX::XMVectorAdd(map.GetLocation(), movement_dir);
		pos = DirectX::XMVectorClamp(pos, DirectX::XMVectorSet(-40.0f, -10.0f, -10.0f, 0.0f), DirectX::XMVectorSet(40.0f, 0.0f, 90.0f, 0.0f));
		//map.SetLocation(pos);
	}
public:
	void Render(auto& canvas)
	{
		draw_obj(canvas, space_shuttle);
		map.Render(canvas, primary_camera);
	}
};