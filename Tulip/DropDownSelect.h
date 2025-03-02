#pragma once
#include"WindowComponent.h"
#include<string>
class DropDownSelect : public WindowComponent
{
public:
	std::function<void(DropDownSelect&)> OnSelect = nullptr;
private:
	std::string current_selection;
public:
	DropDownSelect(Window& wnd, int x, int y, int width, int height) 
		:
	WindowComponent(wnd,WC_COMBOBOX,0, CBS_DISABLENOSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,"Com", x, y, width, height , this)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		si.nMin = 5;
		si.nMax = 100;
		si.nPage = height;
		si.nPos = 0;
		SetScrollInfo(component_handle, SB_VERT, &si, TRUE);
		UpdateWindow(component_handle);
	}
	void PerformCommand(int wparam) override
	{
		if (HIWORD(wparam) == CBN_SELCHANGE)
		{
			if (OnSelect != nullptr)
			{
				auto index = (unsigned int) SendMessage(component_handle, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (index != CB_ERR)
				{
					int len = SendMessage(component_handle, CB_GETLBTEXTLEN, index, 0);
					if (len != CB_ERR)
					{
						std::vector<char> buffer(len + 1);	// +1 for null terminator
						SendMessage(component_handle, (UINT)CB_GETLBTEXT, (WPARAM)index, (LPARAM)buffer.data());
						current_selection = buffer.data();
						OnSelect(*this);
					}
				}
			}
		}
	}
	void AddItem(const std::string& item)
	{
		auto x = SendMessage(component_handle, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)item.c_str());
		UpdateWindow(component_handle);
	}

	std::string GetSelection() const
	{
		return current_selection;
	}
	std::vector<std::string> GetItems() const
	{
		std::vector<std::string> items;
		auto count = (unsigned int) SendMessage(component_handle, CB_GETCOUNT, 0, 0);
		for (auto i = 0u; i < count; i++)
		{
			std::vector<char> buffer(GetWindowTextLength(component_handle) + 1);
			SendMessage(component_handle, (UINT)CB_GETLBTEXT, (WPARAM)i, (LPARAM)buffer.data());
			items.emplace_back(buffer.data());
		}
		return items;
	}
};