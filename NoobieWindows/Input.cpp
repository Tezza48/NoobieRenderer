#include "Input.h"



Input::Input()
{
	lastKBState.resize(KB_SIZE_ELEMENT);
	currentKBState.resize(KB_SIZE_ELEMENT);
}


Input::~Input()
{

}

// Call at the end of game loop
void Input::SwapAndClearKeyboard()
{
	std::swap(lastKBState, currentKBState);

	//std::fill(currentKBState.begin(), currentKBState.end(), false);
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
