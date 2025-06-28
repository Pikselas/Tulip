#pragma once
#include "Object.h"
#include <random>
#include <fstream>
#include <filesystem>

template<typename VertexType>
class ObjLoader : public Object
{
public:
	ObjLoader(auto& canvas , const std::filesystem::path& path)
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open file");
		}

		std::vector<VertexType> vertices;
		std::vector<unsigned int> indices;

		std::mt19937 gen{ std::random_device{}() };
		std::uniform_int_distribution<int> dis{ 0, 255 };

		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty() || line.front() == '#')
				continue;
			std::istringstream iss(line);
			std::string prefix;
			iss >> prefix;
			if (prefix == "v")
			{
				VertexType vertex;
				iss >> vertex.x >> vertex.y >> vertex.z;
				vertex.r = dis(gen);
				vertex.g = dis(gen);
				vertex.b = dis(gen);

				vertices.push_back(vertex);
			}
			else if (prefix == "f")
			{
				if (line.find("//") != std::string::npos)
					continue;
				unsigned int index;
				while (iss >> index)
				{
					indices.push_back(index - 1);
					if (iss.peek() == '/')
					{
						iss.ignore(1, '/');
						unsigned int texIndex;
						iss >> texIndex;
					}
				}
			}
		}
		stride = sizeof(VertexType);
		auto v = std::span<const VertexType>(vertices);
		auto i = std::span(indices);
		Set(canvas, v, i);
	}
};