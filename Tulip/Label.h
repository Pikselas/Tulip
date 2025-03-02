#pragma once
#include<string>
#include"WindowComponent.h"

class Label : public WindowComponent
{
public:
	Label(WindowT auto& Wnd, const char* Name, int x, int y, int w, int h) : WindowComponent(Wnd, "STATIC", 0, WS_CHILD | WS_VISIBLE, Name, x, y, w, h, this) {}
	void PerformCommand(int wparam) override {}
	void SetText(const std::string& Text)
	{
		SetWindowText(component_handle, Text.c_str());
	}
};