#pragma once
#include "ShaderConfiguration.h"
#include "RenderSystem.h"

#include "Terrain.h"
#include "Cube.h"
#include "Sphere.h"
#include "Pyramid.h"
#include "Camera.h"

#include "ObjLoader.h"

struct alignas(16) MatrixData
{
	DirectX::XMMATRIX transform_matrix;
	DirectX::XMMATRIX view_matrix;
};

const auto inp_desc = std::array
{
	InputElemDesc{ "POSITION" ,InputElemDesc::INPUT_FORMAT::FLOAT3 , 0 },
	InputElemDesc{ "COLOR" ,InputElemDesc::INPUT_FORMAT::UINT4 , 12 }
};

inline auto CreateMatrixData(const Object& obj, const Camera& camera)
{
	MatrixData data;
	data.transform_matrix = DirectX::XMMatrixTranspose(obj.GetTransformMatrix());
	data.view_matrix = DirectX::XMMatrixTranspose
	(
		obj.GetTransformMatrix() * camera.GetTransformation() *
		DirectX::XMMatrixPerspectiveLH(1.0f, 600.0f / 800.0f, 1.0f, 100.0f)
	);
	return data;
}

inline auto GetProgramDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, 100);
	std::filesystem::path path = buffer;
	return path.parent_path();
}
