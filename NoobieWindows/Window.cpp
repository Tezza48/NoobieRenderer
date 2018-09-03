#include <iostream>
#include "Window.h"


Window::Window()
{

}

Window::~Window()
{

}

bool Window::Init(const std::wstring windowTitle, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	HMODULE instance = GetModuleHandle(NULL);

	const wchar_t classname[] = L"WindowClass";
	// create the window
	WNDCLASS windowClass;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = instance;
	windowClass.hIcon = LoadIcon(instance, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = classname;

	if (!RegisterClass(&windowClass))
	{
		//MessageBox(0, L"Window failed to register", 0, 0);
		std::cerr << "Window failed to register" << std::endl;
		return false;
	}

	handle = CreateWindow(
		classname, 
		windowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		windowWidth,
		windowHeight,
		0, 
		0, 
		instance, 
		0);

	if (handle == 0)
	{
		//MessageBox(0, L"CreateWindow Failed", 0, 0);
		std::cerr << "CreateWindow Failed" << std::endl;
		return false;
	}

	ShowWindow(handle, SW_SHOW);
	UpdateWindow(handle);
}

bool Window::ShouldWindowClose()
{
	if(message.message != WM_QUIT)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		return false;
	}
	else
		return true;
}

LRESULT CALLBACK WindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE)
			{
				DestroyWindow(window);
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(window, message, wparam, lparam);
}