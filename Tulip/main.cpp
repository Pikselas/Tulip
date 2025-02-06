#include "StandardWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StandardWindow window;
	window.MainLoop();
	return 0;
}