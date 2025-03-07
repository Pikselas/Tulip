#include"PixelShader.h"
#include"VertexShader.h"
#include"ConstantBuffer.h"
#include"KokodaiManager.h"

#include"Terrain.h"
#include"TargetWindow.h"
#include"Cube.h"

struct Factor
{
	float x;
	float y;
	float z;
};

auto GetProgramDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, 100);
	std::filesystem::path path = buffer;
	return path.parent_path();
}

/*Texture& GetTexture(Canvas3D& cnv, int i)
{
	static std::map<std::pair<Canvas3D*, int>, Texture> textures;
	auto texComb = std::make_pair(&cnv, i);
	if (textures.find(texComb) == textures.end())
	{
		Image img{ 500,500 };
		Texture tex{ cnv,img };
		textures.emplace(texComb , tex);
	}
	return textures.at(texComb);
}*/

template<typename T>
concept ObjectT = std::is_base_of_v<Object, T>;

/*template<ObjectT ObjT, int TextureI = 0>
Object BuildObject(Canvas3D& cnv)
{
	Object obj;
	obj = Cube<Canvas3D::VertexType>{ cnv };
	std::vector<InputElemDesc> ieds = {
		{"POSITION" , InputElemDesc::INPUT_FORMAT::FLOAT3 , 0},
		{"COLOR" , InputElemDesc::INPUT_FORMAT::UINT4 , 12}
	};
	const auto programDir = GetProgramDirectory();
	obj.SetVShader(VertexShader(cnv, programDir / "VertexShader.cso",ieds));
	obj.SetPShader(PixelShader(cnv, programDir / "PixelShader.cso"));
	obj.SetCBuffer(ConstantBuffer(cnv, sizeof(DirectX::XMMATRIX)));
	return obj;
}*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	KokodaiManager manager;
	std::mt19937 gen(std::random_device{}());
	//auto start = std::chrono::system_clock::now();
	std::vector<Object> objs;
	objs.reserve(100);

	const auto programDir = GetProgramDirectory();
	std::vector<InputElemDesc> ieds = {
		{"POSITION" , InputElemDesc::INPUT_FORMAT::FLOAT3 , 0},
		{"COLOR" , InputElemDesc::INPUT_FORMAT::UINT4 , 12}
	};

	auto v_Shader = VertexShader(manager.GetCanvas(), programDir / "VertexShader.cso", ieds);
	auto p_Shader = PixelShader(manager.GetCanvas(), programDir / "PixelShader.cso");
	auto c_Buffer = ConstantBuffer(manager.GetCanvas(), sizeof(DirectX::XMMATRIX));

	auto obj = Terrain<Canvas3D::VertexType>{ manager.GetCanvas() };
	obj.SetVShader(v_Shader);
	obj.SetPShader(p_Shader);
	obj.SetCBuffer(c_Buffer);
	obj.SetPosition(0.0f, 0.0f, 0.0f);
	
	//objs.emplace_back(obj);

	for (int i = 0; i < 10; i++)
	{
		obj.SetPosition(std::uniform_real_distribution<float>{-10.0f, 10.0f}(gen),
			std::uniform_real_distribution<float>{-10.0f, 10.0f}(gen),
			std::uniform_real_distribution<float>{-10.0f, 10.0f}(gen)
		);
		objs.emplace_back(obj);
	}

	//manager.GetCanvas().camera.SetPosition(0.0f, 0.0f, 3.5f);
	//manager.GetCanvas().camera.RotatePosition(10, 5, 10);

	
	manager.Run(objs);

	return 0;
}