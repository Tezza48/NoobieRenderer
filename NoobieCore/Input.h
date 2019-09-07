#pragma once
#include <GLFW/glfw3.h>
#include <map>
#include <DirectXMath.h>

class Input
{
public:
private:
	static std::map<int, bool> liveKeyboard;
	static std::map<int, bool> currentKeyboard;
	static std::map<int, bool> lastKeyboard;
	static float currentMouseX;
	static float currentMouseY;
	static float deltaMouseX;
	static float deltaMouseY;
	static float lastMouseX;
	static float lastMouseY;

public:
	/**
		Calculate Mouse deltas and save last input states.
	*/
	static void Update();
	static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static bool GetKey(int key);
	static bool GetKeyDown(int key);
	static DirectX::XMFLOAT2 GetDeltaMouse();
};