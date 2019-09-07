#include "Input.h"

std::map<int, bool> Input::liveKeyboard;
std::map<int, bool> Input::currentKeyboard;
std::map<int, bool> Input::lastKeyboard;
float Input::currentMouseX;
float Input::currentMouseY;
float Input::deltaMouseX;
float Input::deltaMouseY;
float Input::lastMouseX;
float Input::lastMouseY;

void Input::Update()
{
	lastKeyboard = currentKeyboard;
	currentKeyboard = liveKeyboard;

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
		liveKeyboard[key] = true;
		break;
	case GLFW_RELEASE:
		liveKeyboard[key] = false;
	default:
		break;
	}
}

void Input::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	currentMouseX = static_cast<float>(x);
	currentMouseY = static_cast<float>(y);
}

bool Input::GetKey(int key)
{
	return currentKeyboard[key];
}

bool Input::GetKeyDown(int key)
{
	return currentKeyboard[key] && !lastKeyboard[key];
}

DirectX::XMFLOAT2 Input::GetDeltaMouse()
{
	return DirectX::XMFLOAT2(deltaMouseX, deltaMouseY);
}
