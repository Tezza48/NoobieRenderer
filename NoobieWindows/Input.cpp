#include "Input.h"
#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

Input::Input()
{
	liveKBState.resize(MAX_VALUE(char) + 1);
	currentKBState.resize(MAX_VALUE(char) + 1);
	lastKBState.resize(MAX_VALUE(char) + 1);
}


Input::~Input()
{
	
}

void Input::Update()
{
	lastKBState = currentKBState;
	currentKBState = liveKBState;

	lastMouse = currentMouse;
	currentMouse = liveMouse;
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

XMFLOAT2 Input::GetMousePosition() const
{
	return { currentMouse.mouseX, currentMouse.mouseY };
}

XMFLOAT2 Input::GetMouseDelta() const
{
	return { currentMouse.mouseX - lastMouse.mouseX, currentMouse.mouseY - lastMouse.mouseY };
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

