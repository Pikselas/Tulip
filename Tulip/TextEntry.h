#pragma once
#include<vector>
#include"WindowComponent.h"

class TextEntry : public WindowComponent
{
public:
	std::function<void(TextEntry&)> OnTextChange;
public:
	TextEntry(WindowT auto& parent_window, int x, int y, int w, int h, const char* default_text = "")
		: 
		WindowComponent(parent_window, "EDIT", WS_EX_CLIENTEDGE, WS_CHILD | WS_VISIBLE | ES_LEFT| ES_AUTOHSCROLL, default_text, x, y, w, h , this)
	{
	}
	void PerformCommand(int wparam)
	{
		if (HIWORD(wparam) == EN_CHANGE)
		{
			if (OnTextChange)
			{
				OnTextChange(*this);
			}
		}
	}
	void SetText(const std::string& text)
	{
		SetWindowText(component_handle, text.c_str());
	}
	void AppendText(const std::string& text)
	{
		unsigned int nLength = GetWindowTextLength(component_handle);
		SendMessage(component_handle, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
		SendMessage(component_handle, EM_REPLACESEL, 0, (LPARAM)text.c_str());
	}
	const std::string GetText() const
	{
		std::vector<char> text(GetWindowTextLength(component_handle) + 1);
		GetWindowText(component_handle, (LPSTR)text.data(), text.size());
		return std::string(text.data());
	}
};