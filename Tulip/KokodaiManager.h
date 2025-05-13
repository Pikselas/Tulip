#pragma once
#include"Label.h"
#include"StandardWindow.h"
#include"Canvas3D.h"
#include"TextEntry.h"
#include"TextButton.h"
#include"RangeButton.h"
#include"DropDownSelect.h"

#include"TargetWindow.h"
#include"DepthBuffer.h"
#include"Camera.h"

#include "ShaderConfiguration.h"
#include "RenderSystem.h"

#include "Terrain.h"
#include "Cube.h"

#include<mutex>
#include<functional>
class KokodaiManager
{
private:
	StandardWindow mainWindow;
	Canvas3D mainCanvas;
	Camera primary_camera;
private:
	CustomWindow uiWindow;
	CustomWindow renderWindow;
private:
	RenderTarget::Window render_target;
	DepthBuffer depth_buffer;
private:
	ConstantBuffer transformation_buffer;
	Shader::NonTexturedShaderConfig s_config;
	Shader::TerrainShaderConfig t_config;
private:
	Label rotXLabel;
	Label rotYLabel;
	Label camXLabel;
	Label camYLabel;
	Label camZLabel;
	Label rollLabel;
	Label pitchLabel;
	Label yawLabel;
	Label primitiveLabel;
private:
	RangeButton xRot;
	RangeButton yRot;
	RangeButton roll;
	RangeButton pitch;
	RangeButton yaw;
private:
	TextEntry xCam;
	TextEntry yCam;
	TextEntry zCam;
private:
	TextButton xInc;
	TextButton xDec;
	TextButton yInc;
	TextButton yDec;
	TextButton zInc;
	TextButton zDec;
private:
	DropDownSelect primitive;
public:
	std::mutex mtx;
public:
	KokodaiManager();
	~KokodaiManager() = default;
public:
	std::function<void()> OnUpdate = nullptr;
public:
	void Run(std::span<Object> objects);
	Canvas3D& GetCanvas();
	StandardWindow& GetWindow();
};