#pragma once
#include <array>
#include "Object.h"
#include "Reputeless/PerlinNoise.hpp"

template<typename VertexType>
class Terrain : public Object
{
public:
    Terrain(auto& canvas)
    {
        const int xSize = 20;
        const int zSize = 20;

        std::array<VertexType, (xSize + 1)* (zSize + 1)> Vertices = {};
        std::array<unsigned int, 6 * xSize * zSize> Indices = {};

        const siv::PerlinNoise::seed_type seed = 123456u;
        const siv::PerlinNoise perlin{ seed };

		std::mt19937 gen{ std::random_device{}() };
		std::uniform_int_distribution<int> dis{ 0, 255 };

        for (int i = 0; i <= xSize; ++i)
        {
            for (int j = 0; j <= zSize; ++j)
            {
                auto y = perlin.noise2D(i * .3f, j * .3f) * 2.0f;
                Vertices[i * (zSize + 1) + j] = { (float)i , (float)y , (float)j , (unsigned char)dis(gen) , (unsigned char)dis(gen) , (unsigned char)dis(gen)};
            }
        }

        int vert = 0;
        int tris = 0;

        for (int i = 0; i < xSize; ++i)
        {
            for (int j = 0; j < zSize; ++j)
            {
                Indices[tris + 0] = vert + 0;
                Indices[tris + 1] = vert + zSize + 1;
                Indices[tris + 2] = vert + 1;
                Indices[tris + 3] = vert + 1;
                Indices[tris + 4] = vert + zSize + 1;
                Indices[tris + 5] = vert + zSize + 2;

                vert++;
                tris += 6;
            }
            vert++;
        }

        stride = sizeof(VertexType);
        auto v = std::span<const VertexType>(Vertices);
        auto i = std::span(Indices);
        Set(canvas, v, i);
    }
};