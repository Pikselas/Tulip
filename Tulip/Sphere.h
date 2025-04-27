#pragma once
#include <array>
#include "Object.h"
#include <random>

template<typename VertexType>
class Sphere : public Object
{
public:
	Sphere(auto& canvas)
	{
		std::vector<VertexType> vertices;
		std::vector<unsigned int> indices;

		const unsigned int latitude = 12;
		const unsigned int longitude = 24;

		const float radius = 1.0f;

		// PI = 180 degrees 
		// lat angle defines the angle between each latitude line
		const float lat_angle = DirectX::XM_PI / latitude;

		// 2 * PI = 360 degrees
		// lon angle defines the angle between each longitude line
		const float lon_angle = DirectX::XM_2PI / longitude;

		auto base_vertex = DirectX::XMVectorSet(0.0f, 0.0f , radius, 1.0f);

		std::mt19937 gen{ std::random_device{}() };
		std::uniform_int_distribution<int> dis{ 0, 255 };

		for (int iLat = 1; iLat < latitude; ++iLat)
		{
			// rotate the base around x axis
			auto vert = DirectX::XMVector3Transform(base_vertex, DirectX::XMMatrixRotationX(lat_angle * iLat));
			// rotate the base around y axis
			for (int iLon = 0; iLon < longitude; ++iLon)
			{
				auto vertex = DirectX::XMVector3Transform(vert, DirectX::XMMatrixRotationZ(lon_angle * iLon));
				vertices.emplace_back(VertexType
				{
					DirectX::XMVectorGetX(vertex),
					DirectX::XMVectorGetY(vertex),
					DirectX::XMVectorGetZ(vertex),
					(unsigned char)dis(gen),
					(unsigned char)dis(gen),
					(unsigned char)dis(gen)
				});
			}
		}

		auto calc_indx = [latitude, longitude](int iLat, int iLon)
			{
				return (iLat * longitude) + iLon;
			};

		// ignore the poles
		for (int iLat = 0; iLat < latitude - 2; ++iLat)
		{
			for (int iLon = 0; iLon < longitude; ++iLon)
			{
				// the last longitude index is the same as the first one
				// so we need to use modulo operator to wrap around
				// this will create a triangle strip between two latitude lines
				// and two triangles between two latitude lines
				// so we need to create two triangles for each quad
		
				// first triangle
				indices.emplace_back(calc_indx(iLat, iLon));
				indices.emplace_back(calc_indx(iLat + 1, iLon));
				indices.emplace_back(calc_indx(iLat + 1, (iLon + 1) % longitude));
				// second triangle
				indices.emplace_back(calc_indx(iLat, iLon));
				indices.emplace_back(calc_indx(iLat + 1, (iLon + 1) % longitude));
				indices.emplace_back(calc_indx(iLat, (iLon + 1) % longitude));
			}
		}

		// put the poles

		//top pole
		const auto top_pole_index = vertices.size();
		vertices.emplace_back(VertexType
			{
				DirectX::XMVectorGetX(base_vertex),
				DirectX::XMVectorGetY(base_vertex),
				DirectX::XMVectorGetZ(base_vertex),
				(unsigned char)dis(gen),
				(unsigned char)dis(gen),
				(unsigned char)dis(gen)
			});

		//bottom pole
		const auto bottom_pole_index = vertices.size();
		auto bottom_vertex = DirectX::XMVectorNegate(base_vertex);
		vertices.emplace_back(VertexType
			{
				DirectX::XMVectorGetX(bottom_vertex),
				DirectX::XMVectorGetY(bottom_vertex),
				DirectX::XMVectorGetZ(bottom_vertex),
				(unsigned char)dis(gen),
				(unsigned char)dis(gen),
				(unsigned char)dis(gen)
			});

		for (int iLon = 0; iLon < longitude; ++iLon)
		{
			//top
			indices.push_back(top_pole_index);
			indices.push_back(calc_indx(0, iLon));
			indices.push_back(calc_indx(0, (iLon + 1) % longitude));

			//bottom
			indices.push_back(calc_indx(latitude - 2, (iLon + 1) % longitude));
			indices.push_back(calc_indx(latitude - 2, iLon));
			indices.push_back(bottom_pole_index);
		}

		stride = sizeof(VertexType);
		auto v = std::span<const VertexType>(vertices);
		auto i = std::span(indices);
		Set(canvas, v, i);
	}
};