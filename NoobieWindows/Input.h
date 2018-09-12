#pragma once

#include <vector>

#define MAX_VALUE(x) (1 << sizeof(x) * 8) - 1

using std::vector;

class Input
{
private:
	vector<bool> lastKBState;
	vector<bool> currentKBState;

public:
	// https://docs.microsoft.com/en-gb/windows/desktop/inputdev/virtual-key-codes
	enum KB : unsigned char
	{
		KB_BACKSPACE = 0x08,
		KB_TAB,
		KB_RETURN = 0x0D,
		KB_SHIFT = 0x10,
		KB_CONTROL,
		KB_ALT,
		KB_PAUSE,
		KB_CAPSLOCK,
		KB_ESCAPE = 0x1B,
		// MISC
		KB_SPACE = 0x20,
		KB_PG_UP,
		KB_PG_DOWN,
		KB_END,
		KB_HOME,
		KB_LEFT,
		KB_RIGHT,
		KB_UP,
		KB_DOWN,
		KB_SELECT,
		KB_PRINT,
		KB_EXECUTE,
		KB_PRINTSCREEN,
		KB_INSERT,
		KB_DELETE,
		KB_HELP,// NUMBERS
		KB_0 = 0x30,
		KB_1,
		KB_2,
		KB_3,
		KB_4,
		KB_5,
		KB_6,
		KB_7,
		KB_8,
		KB_9,// LETTERS
		KB_A = 0x41,
		KB_B,
		KB_C,
		KB_D,
		KB_E,
		KB_F,
		KB_G,
		KB_H,
		KB_I,
		KB_J,
		KB_K,
		KB_L,
		KB_M,
		KB_N,
		KB_O,
		KB_P,
		KB_Q,
		KB_R,
		KB_S,
		KB_T,
		KB_U,
		KB_V,
		KB_W,
		KB_X,
		KB_Y,
		KB_Z,// NUMPAD
		KB_NUMPAD_0 = 0x60,
		KB_NUMPAD_1,
		KB_NUMPAD_2,
		KB_NUMPAD_3,
		KB_NUMPAD_4,
		KB_NUMPAD_5,
		KB_NUMPAD_6,
		KB_NUMPAD_7,
		KB_NUMPAD_8,
		KB_NUMPAD_9,// MATHS KEYS
		KB_ASTERISK,
		KB_PLUS,
		KB_OR,
		KB_MINUS,
		KB_PERIOD, // |
		KB_DIVIDE,// F KEYS
		KB_F1 = 0x70,
		KB_F2,
		KB_F3,
		KB_F4,
		KB_F5,
		KB_F6,
		KB_F7,
		KB_F8,
		KB_F9,
		KB_F10,
		KB_F11,
		KB_F12,// SKIPPING 13 - 24 // MORE MISC
		KB_NUMLOCK = 0x90,
		KB_SCROLL,
		KB_SHIFT_LEFT = 0xA0,
		KB_SHIFT_RIGHT,
		KB_CONTROL_LEFT,
		KB_CONTROL_RIGHT,
		KB_SIZE_ELEMENT
	};

public:
	Input();
	~Input();

	void Update();

	void WndProcKeyPresed(KB keycode);
	void WndProcKeyReleased(KB keycode);

	bool GetKey(KB keycode);
	bool GetKeyUp(KB keycode);
	bool GetKeyDown(KB keycode);
	
	//inline const vector<bool> & GetCurrentKBState() const { return currentKBState; }
	//inline const vector<bool> & GetLastKBState() const { return lastKBState; }

};