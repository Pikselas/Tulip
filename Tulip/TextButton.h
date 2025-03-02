#pragma once
#include<string>
#include"WindowComponent.h"

class TextButton : public WindowComponent
{
public:
	std::function<void(TextButton& button)> OnClick = nullptr;
	std::function<void(TextButton& button)> OnDblClick = nullptr;
public:
	TextButton(WindowT auto& parent_window, const std::string& Title, int pos_x, int pos_y, int width, int height)
		:
		WindowComponent(parent_window, "Button", 0 , WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, Title.c_str(), pos_x, pos_y, width, height, this)
	{}
	void PerformCommand(int wparam) override
	{
		if (HIWORD(wparam) == BN_CLICKED && OnClick != nullptr)
		{
			OnClick(*this);
		}
		else if (HIWORD(wparam) == BN_DBLCLK && OnDblClick != nullptr)
		{
			OnDblClick(*this);
		}
	}
};