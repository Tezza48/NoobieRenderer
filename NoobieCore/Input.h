#pragma once
#include <GLFW/glfw3.h>
#include <map>

class Input
{
public:
private:
	static std::map<int, bool> lastKeyboard;

public:
	static void Update();
	static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static bool GetKey(int key);
};