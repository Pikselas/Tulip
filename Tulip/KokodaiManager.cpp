#include "KokodaiManager.h"

KokodaiManager::KokodaiManager()
	:
mainWindow("Tulip - Engine",1200,610),

uiWindow(&mainWindow,0,"",WS_CHILD,810,100,400, 400),
renderWindow(&mainWindow, 0, "", WS_CHILD, 5, 5, 800, 600),
render_target(mainCanvas.Device.Get(), renderWindow.window_handle, 800, 600),
depth_buffer(mainCanvas.Device.Get(), 800, 600),

s_config(mainCanvas.Device.Get()),
transformation_buffer(mainCanvas.Device.Get(), sizeof(DirectX::XMMATRIX)),

rotXLabel(uiWindow, "rot-X", 10, 10, 30, 20),
rotYLabel(uiWindow, "rot-Y", 10, 40, 30, 20),
camXLabel(uiWindow, "X", 15, 70, 15, 20),
camYLabel(uiWindow, "Y", 115, 70, 15, 20),
camZLabel(uiWindow, "Z", 225, 70, 15, 20),
rollLabel(uiWindow, "Roll", 10, 100, 30, 20),
pitchLabel(uiWindow, "Pitch", 10, 130, 30, 20),
yawLabel(uiWindow, "Yaw", 10, 160, 30, 20),
primitiveLabel(uiWindow, "Primitive", 10, 190, 70, 20),

xRot(uiWindow, 0, 360, 50, 10, 300, 20),
yRot(uiWindow, 0, 360, 50, 40, 300, 20),
roll(uiWindow, 0, 360, 50, 100, 300, 20),
pitch(uiWindow, 0, 360, 50, 130, 300, 20),
yaw(uiWindow, 0, 360, 50, 160, 300, 20),

xCam(uiWindow, 35, 70, 35, 20),
yCam(uiWindow, 135, 70, 35, 20),
zCam(uiWindow, 245, 70, 35, 20),

xInc(uiWindow, "+", 75, 70, 35, 10),
xDec(uiWindow, "-", 75, 80, 35, 10),
yInc(uiWindow, "+", 175, 70, 35, 10),
yDec(uiWindow, "-", 175, 80, 35, 10),
zInc(uiWindow, "+", 285, 70, 35, 10),
zDec(uiWindow, "-", 285, 80, 35, 10),

primitive(uiWindow, 80, 190, 100, 100)

{


	primitive.AddItem("Triangle");
	primitive.AddItem("Line");
	primitive.AddItem("Point");

	primitive.OnSelect = [&](DropDownSelect& dd)
	{
		if (const auto selected = dd.GetSelection(); selected == "Triangle")
			mainCanvas.SetPrimitiveTopology(Canvas3D::PrimitiveTopology::Triangle);
		else if (selected == "Line")
			mainCanvas.SetPrimitiveTopology(Canvas3D::PrimitiveTopology::Line);
		else if (selected == "Point")
			mainCanvas.SetPrimitiveTopology(Canvas3D::PrimitiveTopology::Point);
	};

	xRot.OnSlide = [&](RangeButton& rb)
	{
		primary_camera.RotateOrientation(rb.GetCurrentPos(), yRot.GetCurrentPos());
		OutputDebugString((std::to_string(rb.GetCurrentPos()) + "," + std::to_string(yRot.GetCurrentPos()) + "\n").c_str());
	};
	yRot.OnSlide = [&](RangeButton& rb)
	{
		primary_camera.RotateOrientation(xRot.GetCurrentPos(), rb.GetCurrentPos());
	};
	roll.OnSlide = [&](RangeButton& rb)
	{
		primary_camera.RotatePosition(rb.GetCurrentPos(), pitch.GetCurrentPos(), yaw.GetCurrentPos());
	};
	pitch.OnSlide = [&](RangeButton& rb)
	{
		primary_camera.RotatePosition(roll.GetCurrentPos(), rb.GetCurrentPos(), yaw.GetCurrentPos());
	};
	yaw.OnSlide = [&](RangeButton& rb)
	{
		primary_camera.RotatePosition(roll.GetCurrentPos(), pitch.GetCurrentPos(), rb.GetCurrentPos());
	};

	auto GetF = [](const std::string& s)
	{
		try
		{
			return std::stof(s);
		}
		catch (const std::exception&)
		{
			return 0.1f;
		}
	};
	
	zCam.SetText("1.0");
	
	zCam.OnTextChange = [&](TextEntry& te)
	{
		primary_camera.SetZ(GetF(zCam.GetText()));
	};

	xInc.OnClick = [&](TextButton& b)
	{
		xCam.SetText(std::to_string(GetF(xCam.GetText()) + 1.0f));
	};
	xDec.OnClick = [&](TextButton& b)
	{
		xCam.SetText(std::to_string(GetF(xCam.GetText()) - 1.0f));
	};
	yInc.OnClick = [&](TextButton& b)
	{
		yCam.SetText(std::to_string(GetF(yCam.GetText()) + 1.0f));
	};
	yDec.OnClick = [&](TextButton& b)
	{
		yCam.SetText(std::to_string(GetF(yCam.GetText()) - 1.0f));
	};
	zInc.OnClick = [&](TextButton& b)
	{
		zCam.SetText(std::to_string(GetF(zCam.GetText()) + 1.0f));
	};
	zDec.OnClick = [&](TextButton& b)
	{
		zCam.SetText(std::to_string(GetF(zCam.GetText()) - 1.0f));
	};

	mainCanvas.SetRenderTarget(render_target);
	depth_buffer.Bind(mainCanvas.ImmediateContext.Get());
	s_config.SetConstantBuffer(transformation_buffer);
	s_config.BindToContext(mainCanvas.ImmediateContext.Get());
}

void KokodaiManager::Run(std::span<Object> objects)
{
	auto time_point = std::chrono::system_clock::now();
	while (mainWindow.IsOpen())
	{
		const auto now = std::chrono::system_clock::now();
		const auto elapsed = std::chrono::duration<float>(now - time_point).count();
		
		//DirectX::XMVECTOR light_direction = DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f);
		//light_direction = DirectX::XMVector3Transform(light_direction, DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, elapsed));

		mtx.lock();
		render_target.Clear();
		depth_buffer.Clear(mainCanvas.ImmediateContext.Get());
	
		for (auto& obj : objects)
		{
			if (obj.OnUpdate)
			{
				obj.OnUpdate(obj);
			}

			obj.SetCBuffer(transformation_buffer);

			const auto matrix = DirectX::XMMatrixTranspose
			(
				obj.GetTansformMatrix() * primary_camera.GetTransformation() *
				DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 1.0f, 40.0f)
			);
			mainCanvas.UpdateCbuff(transformation_buffer.GetBuffer().Get(), matrix);
			mainCanvas.DrawObject(obj,primary_camera);
		}
		//mainCanvas.PresentOnWindow();
		render_target.RenderFrame();
		mtx.unlock();
		if (OnUpdate)
			OnUpdate();
		//mainWindow.Redraw();
		CustomWindow::DispatchWindowEventsNonBlocking();
	}
}

Canvas3D& KokodaiManager::GetCanvas()
{
	return mainCanvas;
}

StandardWindow& KokodaiManager::GetWindow()
{
	return mainWindow;
}
