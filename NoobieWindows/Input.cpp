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
	std::swap(lastKBState, currentKBState);
	std::fill(currentKBState.begin(), currentKBState.end(), false);

	//for (unsigned char i = 0; i < MAX_VALUE(char); i++)
	//{
	//	if ((bool)lastKBState[i] != (bool)currentKBState[i])
	//	{
	//		bool state = currentKBState[i];
	//		printf("%c: %s\n", i, state ? "true": "false");
	//	}
	//}
}

void Input::WndProcKeyPresed(KB keycode)
{
	// if the key is in bounds of my enums then set it
	currentKBState.at(keycode) = true;
}

void Input::WndProcKeyReleased(KB keycode)
{
	currentKBState.at(keycode) = false;
}

bool Input::GetKey(KB keycode)
{
	return currentKBState[keycode];
}

bool Input::GetKeyUp(KB keycode)
{
	return !currentKBState[keycode] && lastKBState[keycode];
}

bool Input::GetKeyDown(KB keycode)
{
	return currentKBState[keycode] && !lastKBState[keycode];
}
