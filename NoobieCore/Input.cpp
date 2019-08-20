#include "Input.h"

std::map<int, bool> Input::lastKeyboard;

void Input::Update()
{
	// Swap maps
}

void Input::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	// Do stuff with the keys
	switch (action)
	{
	case GLFW_PRESS:
		lastKeyboard[key] = true;
		break;
	case GLFW_RELEASE:
		lastKeyboard[key] = false;
	default:
		break;
	}
}

void Input::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	printf("\r%f, %f", x, y);
}

bool Input::GetKey(int key)
{
	return lastKeyboard[key];
}
