#pragma once
#include"Object.h"

template<typename VertexType>
class Cube : public Object
{
public:
	static constexpr VertexType Vertices[] = {
		//front
		{-1.0f , 1.0f , -1.0f , 100 , 200 , 150},
		{ 1.0f , 1.0f , -1.0f , 200 , 100 , 50},
		{-1.0f , -1.0f , -1.0f , 50 , 60 , 40},
		{ 1.0f , -1.0f , -1.0f , 100 , 120 , 130},

		//left
		{-1.0f ,1.0f , 1.0f , 200 , 0 , 10 },
		{-1.0f ,1.0f , -1.0f , 80 , 100 , 25},
		{-1.0f ,-1.0f , 1.0f , 220 , 190 , 34 },
		{-1.0f ,-1.0f , -1.0f , 100 , 123 , 78},

		//top
		{-1.0f ,1.0f , 1.0f , 99 , 98 , 97},
		{ 1.0f ,1.0f , 1.0f , 213 , 183 , 65},
		{-1.0f ,1.0f , -1.0f ,123 , 87 , 90 },
		{ 1.0f ,1.0f , -1.0f , 14 , 78 , 36 },

		//back
		{-1.0f , 1.0f , 1.0f , 123 , 67 , 100},
		{ 1.0f , 1.0f , 1.0f , 220 , 78 , 13},
		{-1.0f , -1.0f , 1.0f , 90 , 54 , 67},
		{ 1.0f , -1.0f , 1.0f , 132 , 78 , 21},

		//right
		{1.0f ,1.0f , 1.0f , 34 , 78 , 90 },
		{1.0f ,1.0f , -1.0f , 123 , 89 , 36 },
		{1.0f ,-1.0f , 1.0f , 24 , 63 , 217 },
		{1.0f ,-1.0f , -1.0f , 24 , 80 , 90 },

		//bottom
		{-1.0f ,-1.0f , -1.0f , 24 , 99 , 213},
		{ 1.0f ,-1.0f , -1.0f , 47 , 76 , 56 },
		{-1.0f ,-1.0f , 1.0f , 90 , 12 , 64 },
		{ 1.0f ,-1.0f , 1.0f , 124 , 76 , 34}
	};
	static constexpr unsigned int Indices[] ={
		2 , 0 ,1,
		2 , 1 ,3,

		6 , 4, 5,
		5 , 7 ,6,

		10 , 8 , 9,
		9 , 11, 10,

		13 , 12 , 14,
		14 , 15 , 13,

		18 , 17 , 16,
		19 , 17 , 18,

		20 , 21 , 22,
		23 , 22 , 21
	};
public:
	Cube(auto& canvas)
	{
		stride = sizeof(VertexType);
		auto v = std::span<const VertexType>(Vertices);
		auto i = std::span(Indices);
		Set(canvas, v, i);
	}
};