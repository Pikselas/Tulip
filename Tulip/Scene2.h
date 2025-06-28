#pragma once
#include "CommonSceneHelpers.h"

class Collider
{
public:
	virtual DirectX::XMVECTOR GetPosition() const = 0;
	virtual DirectX::XMVECTOR GetNonCollidingPoint(DirectX::XMVECTOR dir) const = 0;
public:
	static bool IsColliding(const Collider& collider, const DirectX::XMVECTOR point)
	{
		const auto pos = collider.GetPosition();
		const auto non_colliding_point = collider.GetNonCollidingPoint(DirectX::XMVectorSubtract(point, pos));
	}
};

class SphereCollider : public Collider
{
public:
	DirectX::XMVECTOR position;
	float radius;
	SphereCollider(DirectX::XMVECTOR pos, float r)
		: position(pos), radius(r) {
	}
	DirectX::XMVECTOR GetPosition() const override
	{
		return position;
	}
	bool IsColliding(const DirectX::XMVECTOR point) const
	{
		const auto diff = DirectX::XMVectorSubtract(point, position);
		const auto distance_squared = DirectX::XMVector3LengthSq(diff);
		return DirectX::XMVector3LessOrEqual(distance_squared, DirectX::XMVectorReplicate(radius * radius));
	}
	DirectX::XMVECTOR GetNonCollidingPoint(DirectX::XMVECTOR dir) const override
	{
		return DirectX::XMVectorAdd(position, DirectX::XMVectorScale(dir, radius));
	}
	DirectX::XMVECTOR GetNonCollidingPoint(const Object& obj) const
	{
		const auto pos = obj.GetPosition();
		const auto dir = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(pos, position));
		return GetNonCollidingPoint(dir);
	}
};

class Scene2
{
private:
	Shader::PixelShader voiater_shader;
	Shader::PixelShader asteroid_shader;
	Shader::VertexShader vertex_shader;
private:
	ConstantBuffer transformation_buffer;
	Shader::ShaderConfiguration s_config;
	Shader::ShaderConfiguration a_config;
public:
	Camera primary_camera;
private:
	struct Boid
	{
		Object obj;
		float collision_angle;
		DirectX::XMVECTOR velocity;
	};
	float z_movement_speed = -0.050f;
private:
	Object asteroid;
	std::array<Boid, 500> boids;
public:
	void SetMovementLocation(int , int)
	{

	}
public:
	Scene2(auto& canvas)
		:
		vertex_shader(canvas.Device.Get(), GetProgramDirectory() / "VertexShader.cso", std::span<const InputElemDesc>{ inp_desc }),
		voiater_shader(canvas.Device.Get(), GetProgramDirectory() / "VoiaterPixelShader.cso"),
		asteroid_shader(canvas.Device.Get(), GetProgramDirectory() / "AsteroidPixelShader.cso"),
		transformation_buffer(canvas.Device.Get(), sizeof(MatrixData))
	{
		auto pyramid = Pyramid<Canvas3D::VertexType>{ canvas };
		asteroid = Sphere<Canvas3D::VertexType>{ canvas };
		
		vertex_shader.SetConstBuffer(transformation_buffer.GetBuffer());

		s_config.AddShader(vertex_shader);
		s_config.AddShader(voiater_shader);
		pyramid.SetShaderConfig(s_config);

		pyramid.SetPosition(DirectX::XMVectorZero());
		pyramid.SetTransformation(DirectX::XMMatrixScaling(0.03 , 0.03 , 0.03));

		a_config.AddShader(vertex_shader);
		a_config.AddShader(asteroid_shader);

		asteroid.SetShaderConfig(s_config);
		asteroid.SetPosition(DirectX::XMVectorSet(0.0f, 0.0f, -4.0f, 1.0f));
		asteroid.SetTransformation(DirectX::XMMatrixScaling(0.15f, 0.15f, 0.15f));

		//primary_camera.SetZ(2.0f);
		std::uniform_real_distribution<float> dist(0.0f, DirectX::XM_2PI);
		std::uniform_real_distribution<float> zDist(0.0f, 20.0f);
		std::mt19937 rng(std::random_device{}());
		for (auto& boid : boids)
		{
			boid.obj = pyramid;
			boid.obj.SetPosition(DirectX::XMVectorSetZ(pyramid.GetPosition() ,zDist(rng)));
			boid.velocity = DirectX::XMVectorSet(0.0f, 0.0f, z_movement_speed, 0.0f);
			boid.collision_angle = dist(rng);
			//boid.obj.SetTransformation(DirectX::XMMatrixRotationY(dist(rng)));
			//boid.obj.SetPosition(DirectX::XMVectorSet(dist(rng), dist(rng), 0.0f, 1.0f));
		}
	}
	void Update(float delta_time)
	{
		if (primary_camera.GetZ() < 2.0f)
			return;
		SphereCollider collider{DirectX::XMVectorSet(0.0f,0.0f,-20.0f,1.0f) , 10};
		
		for (auto& boid : boids)
		{
			boid.obj.SetPosition(DirectX::XMVectorAdd(boid.obj.GetPosition(), boid.velocity));
			if (collider.IsColliding(boid.obj.GetPosition()))
			{
				auto v = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f , 0.01f , 0.0f, 1.0f), DirectX::XMMatrixRotationZ(boid.collision_angle));
				boid.velocity = DirectX::XMVectorSetZ(v , z_movement_speed);
			}

			if (!DirectX::XMVector3Equal(boid.velocity , DirectX::XMVectorSet(0.0,0.0, z_movement_speed, 0.0f)))
			{
				auto surf_pos = collider.GetNonCollidingPoint(boid.obj);
				surf_pos = DirectX::XMVectorSetZ(surf_pos, DirectX::XMVectorGetZ(boid.obj.GetPosition()));
				auto next_surf_pos = collider.GetNonCollidingPoint(DirectX::XMVectorAdd(surf_pos , boid.velocity));
				auto rot = std::atan2(std::abs(DirectX::XMVectorGetY(next_surf_pos) - DirectX::XMVectorGetY(surf_pos)), std::abs(DirectX::XMVectorGetX(next_surf_pos) - DirectX::XMVectorGetX(surf_pos)));
				boid.obj.RotateFixedPoint(rot - DirectX::XM_PIDIV2, 0.0f, 0.0f);
				boid.obj.SetPosition(surf_pos);
			}
		}
	}

	void draw_obj(auto& canvas, const Object& obj) const
	{
		const auto matrix = CreateMatrixData(obj, primary_camera);
		canvas.UpdateCbuff(transformation_buffer.GetBuffer().Get(), matrix);
		obj.GetShaderConfig()->BindToContext(canvas.ImmediateContext.Get());
		canvas.DrawObject(obj, primary_camera);
	}

	void Render(auto& canvas) const
	{
		draw_obj(canvas, asteroid);
		for (const auto& boid : boids)
		{
			draw_obj(canvas, boid.obj);
		}
	}
};