#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include<functional>

class Window;

template<typename T>
concept WindowT = std::is_same_v<T, Window>;

class WindowComponent
{
	friend Window;
protected:
	HWND component_handle;
protected:
	WindowComponent(WindowT auto& Wnd , const char * Class , DWORD Styles , DWORD Options , const char* Name , int x , int y , int w , int h , WindowComponent * Component)
	{
		component_handle = CreateWindowEx(Styles, Class, Name, Options, x, y, w, h, Wnd.window_handle, nullptr, GetModuleHandle(nullptr), nullptr);
		SetWindowLongPtr(component_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(Component));
	}
	virtual void PerformCommand(int wparam) = 0;
};