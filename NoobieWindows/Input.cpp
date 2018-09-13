#include "Input.h"
#include <Windows.h>


Input::Input()
{
	lastKBState.resize(MAX_VALUE(char) + 1);
	currentKBState.resize(MAX_VALUE(char) + 1);
}


Input::~Input()
{
	
}

void Input::Update()
{
	lastKBState = currentKBState;
	lastMouse = currentMouse;
}

bool Input::GetKey(KB keycode) const
{
	return currentKBState[keycode];
}

bool Input::GetKeyUp(KB keycode) const
{
	return !currentKBState[keycode] && lastKBState[keycode];
}

bool Input::GetKeyDown(KB keycode) const
{
	return currentKBState[keycode] && !lastKBState[keycode];
}

vector<float> Input::GetMousePosition() const
{
	return vector<float>{currentMouse.mouseX, currentMouse.mouseY};
}

vector<float> Input::GetMouseDelta() const
{
	return vector<float>{ currentMouse.mouseX - lastMouse.mouseX, currentMouse.mouseY - lastMouse.mouseY };
}

bool Input::GetMouseButton(unsigned int button) const
{
	return currentMouse.button[button];
}

bool Input::GetMouseButtonUp(unsigned int button) const
{
	return !currentMouse.button[button] && lastMouse.button[button];
}

bool Input::GetMouseButtonDown(unsigned int button) const
{
	return currentMouse.button[button] && !lastMouse.button[button];
}

