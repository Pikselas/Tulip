#pragma once
#include"Object.h"

class TexturedCube : public Object
{
private:
	struct VertexType
	{
		float x, y, z;
		float u , v;
	};
	static VertexType Vertices[];
	static const unsigned int Indices[];
public:
	TexturedCube(auto& canvas)
	{
		stride = sizeof(VertexType);
		auto v = std::span<const VertexType>(Vertices);
		auto i = std::span(Indices);
		Set(canvas, v, i);
	}
};

TexturedCube::VertexType TexturedCube::Vertices[] = {
	 
	//front
	{-1.0f , 1.0f , -1.0f , 0.0f , 0.0f},
	{ 1.0f , 1.0f , -1.0f , 1.0f , 0.0f},
	{-1.0f , -1.0f , -1.0f , 0.0f , 1.0f},
	{ 1.0f , -1.0f , -1.0f , 1.0f , 1.0f},
	
	//left
	{-1.0f ,1.0f , 1.0f , 0.0f , 0.0f },
	{-1.0f ,1.0f , -1.0f , 1.0f , 0.0f },
	{-1.0f ,-1.0f , 1.0f , 0.0f , 1.0f },
	{-1.0f ,-1.0f , -1.0f , 1.0f , 1.0f },

	//top
	{-1.0f ,1.0f , 1.0f , 0.0f , 0.0f },
	{ 1.0f ,1.0f , 1.0f , 1.0f , 0.0f },
	{-1.0f ,1.0f , -1.0f , 0.0f , 1.0f },
	{ 1.0f ,1.0f , -1.0f , 1.0f , 1.0f },

	//back
	{-1.0f , 1.0f , 1.0f , 0.0f , 0.0f},
	{ 1.0f , 1.0f , 1.0f , 1.0f , 0.0f},
	{-1.0f , -1.0f , 1.0f , 0.0f , 1.0f},
	{ 1.0f , -1.0f , 1.0f , 1.0f , 1.0f},

	//right
	{1.0f ,1.0f , 1.0f , 0.0f , 0.0f },
	{1.0f ,1.0f , -1.0f , 1.0f , 0.0f },
	{1.0f ,-1.0f , 1.0f , 0.0f , 1.0f },
	{1.0f ,-1.0f , -1.0f , 1.0f , 1.0f },

	//bottom
	{-1.0f ,-1.0f , -1.0f , 0.0f , 0.0f },
	{ 1.0f ,-1.0f , -1.0f , 1.0f , 0.0f },
	{-1.0f ,-1.0f , 1.0f , 0.0f , 1.0f },
	{ 1.0f ,-1.0f , 1.0f , 1.0f , 1.0f },

};

const unsigned int TexturedCube::Indices[] = {

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
