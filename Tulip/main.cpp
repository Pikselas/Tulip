
#include"KokodaiManager.h"

#include"Cube.h"
#include"Sphere.h"
//#include"Terrain.h"

struct Factor
{
	float x;
	float y;
	float z;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	KokodaiManager manager;
	std::mt19937 gen(std::random_device{}());
	auto start = std::chrono::system_clock::now();
	std::vector<Object> objs;
	objs.reserve(100);

	std::uniform_real_distribution<float>pos {-10.0f, 10.0f};

	//auto objC = Cube<Canvas3D::VertexType>{manager.GetCanvas()};
	//auto objT = Terrain<Canvas3D::VertexType>{ manager.GetCanvas() };
	//auto objS = Sphere<Canvas3D::VertexType>{manager.GetCanvas()};

	//objT.RotatePositional((unsigned short)0 ,0,180);
	//objT.SetPosition(-10.0, 2.0, 0.0);
	//objT.OnUpdate = [](Object& obj)
	//	{
	//		//auto d = std::chrono::duration<float>(start - std::chrono::system_clock::now()).count();
	//		auto pos = obj.GetPosition();
	//		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorSet(0.0f, 0.0f, -0.05f, 0.0f));
	//		if(DirectX::XMVectorGetZ(pos) < -20.0f)
	//			pos = DirectX::XMVectorSet(-10.0f, 2.0f, 10.0f, 0.0f);
	//		obj.SetPosition(pos);
	//		//obj.SetPosition;
	//	};
	//objs.emplace_back(objT);
	//objT.SetPosition(-10.0, 2.0, 20.0);
	//objT.OnUpdate = [](Object& obj)
	//	{
	//		//auto d = std::chrono::duration<float>(start - std::chrono::system_clock::now()).count();
	//		auto pos = obj.GetPosition();
	//		pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorSet(0.0f, 0.0f, -0.05f, 0.0f));
	//		if (DirectX::XMVectorGetZ(pos) < -20.0f)
	//			pos = DirectX::XMVectorSet(-10.0f, 2.0f, 30.0f, 0.0f);
	//		obj.SetPosition(pos);
	//		//obj.SetPosition;
	//	};
	//objs.emplace_back(objT);
	/*for (int i = 0; i < 100; i++)
	{
		Object obj = objS;
		

		obj.SetPosition(pos(gen),pos(gen),pos(gen));

		std::uniform_real_distribution<float> rotation{ -1.0f, 1.0f };

		auto f = Factor{ rotation(gen), rotation(gen),rotation(gen) };
		obj.OnUpdate = [&, fact = f](Object& obj)
			{
				auto d = std::chrono::duration<float>(start - std::chrono::system_clock::now()).count();
				obj.RotatePositional(fact.x * d, fact.y * d, fact.z * d);
				obj.RotateFixedPoint(fact.x * d, fact.y * d, fact.z * d);
			};
		
		objs.emplace_back(obj);
	}*/

	//manager.GetCanvas().camera.SetPosition(0.0f, 0.0f, 3.5f);
	//manager.GetCanvas().camera.RotatePosition(10, 5, 10);

	
	manager.Run(objs);

	return 0;
}