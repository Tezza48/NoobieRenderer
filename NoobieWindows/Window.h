#pragma once
#include <Windows.h>
#include <string>

LRESULT CALLBACK WindowProc(HWND window, unsigned int message, WPARAM wparam, LPARAM lparam);

class Window
{
private:
	HWND handle;
	MSG message = { 0 };
	int windowWidth;
	int windowHeight;
	bool isWindowed = true;
public:
	Window();
	~Window();

	bool Init(const std::wstring windowTitle, int width, int height);
	bool ShouldWindowClose();

	HWND GetHandle() const { return handle; }
	int GetWindowWidth() const { return windowWidth; }
	int GetWindowHeight() const { return windowHeight; }
	float GetAspectRatio() const {return static_cast<float>(windowWidth)/static_cast<float>(windowHeight); }
	bool GetIsWindowed() const { return isWindowed; }
};

