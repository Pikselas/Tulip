#pragma once
#include"Object.h"

class PaperTex : public Object
{
private:
	struct VertexType
	{
		float x, y, z;
		float u, v;
	};
public:
	static constexpr VertexType Vertices[] = {
		//front
		{-1.0f , 1.0f , -1.0f , 0.0f , 0.0f},
		{ 1.0f , 1.0f , -1.0f , 1.0f , 0.0f},
		{-1.0f , -1.0f , -1.0f , 0.0f , 1.0f},
		{ 1.0f , -1.0f , -1.0f , 1.0f , 1.0f},
	};
	static constexpr unsigned int Indices[] = {
		2 , 0 ,1,
		2 , 1 ,3,
	};
public:
	PaperTex(auto& canvas)
	{
		stride = sizeof(VertexType);
		auto v = std::span<const VertexType>(Vertices);
		auto i = std::span(Indices);
		Set(canvas, v, i);
	}
};