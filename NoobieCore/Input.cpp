#include "Input.h"

std::map<int, bool> Input::lastKeyboard;
float Input::currentMouseX;
float Input::currentMouseY;
float Input::deltaMouseX;
float Input::deltaMouseY;
float Input::lastMouseX;
float Input::lastMouseY;

void Input::Update()
{
	// Swap maps
	deltaMouseX = currentMouseX - lastMouseX;
	deltaMouseY = currentMouseY - lastMouseY;

	lastMouseX = currentMouseX;
	lastMouseY = currentMouseY;
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
	currentMouseX = x;
	currentMouseY = y;
}

bool Input::GetKey(int key)
{
	return lastKeyboard[key];
}

DirectX::XMFLOAT2 Input::GetDeltaMouse()
{
	return DirectX::XMFLOAT2(deltaMouseX, deltaMouseY);
}
