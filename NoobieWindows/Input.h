#pragma once

#include <vector>
#include <DirectXMath.h>
#include <GLFW/glfw3.h>

#define MAX_GLFW_KEY 348

#define MAX_VALUE(x) (1 << sizeof(x) * 8) - 1

using std::vector;

class Input
{
public:
	struct MouseData
	{
		union
		{
			struct
			{
				float mousePos[2];
				bool button[3];
			};
			struct
			{
				float mouseX;
				float mouseY;
				bool lButton;
				bool rButton;
				bool mButton;
			};
		};
	};
	// https://docs.microsoft.com/en-gb/windows/desktop/inputdev/virtual-key-codes
	enum KB
	{
		KB_BACKSPACE = GLFW_KEY_BACKSPACE,
		KB_TAB = GLFW_KEY_TAB,
		KB_RETURN = GLFW_KEY_ENTER,
		KB_SHIFT = GLFW_KEY_LEFT_SHIFT,
		KB_CONTROL = GLFW_KEY_LEFT_CONTROL,
		KB_ALT = GLFW_KEY_LEFT_ALT,
		KB_PAUSE = GLFW_KEY_PAUSE,
		KB_CAPSLOCK = GLFW_KEY_CAPS_LOCK,
		KB_ESCAPE = GLFW_KEY_ESCAPE,
		// MISC
		KB_SPACE = GLFW_KEY_SPACE,
		KB_PG_UP = GLFW_KEY_PAGE_UP,
		KB_PG_DOWN = GLFW_KEY_PAGE_DOWN,
		KB_END = GLFW_KEY_END,
		KB_HOME = GLFW_KEY_HOME,
		KB_LEFT = GLFW_KEY_LEFT,
		KB_RIGHT = GLFW_KEY_RIGHT,
		KB_UP = GLFW_KEY_UP,
		KB_DOWN = GLFW_KEY_DOWN,
		KB_PRINTSCREEN = GLFW_KEY_PRINT_SCREEN,
		KB_INSERT = GLFW_KEY_INSERT,
		KB_DELETE = GLFW_KEY_DELETE,
		KB_0 = GLFW_KEY_0,
		KB_1 = GLFW_KEY_1,
		KB_2 = GLFW_KEY_2,
		KB_3 = GLFW_KEY_3,
		KB_4 = GLFW_KEY_4,
		KB_5 = GLFW_KEY_5,
		KB_6 = GLFW_KEY_6,
		KB_7 = GLFW_KEY_7,
		KB_8 = GLFW_KEY_8,
		KB_9 = GLFW_KEY_9,
		KB_A = GLFW_KEY_A,
		KB_B = GLFW_KEY_B,
		KB_C = GLFW_KEY_C,
		KB_D = GLFW_KEY_D,
		KB_E = GLFW_KEY_E,
		KB_F = GLFW_KEY_F,
		KB_G = GLFW_KEY_G,
		KB_H = GLFW_KEY_H,
		KB_I = GLFW_KEY_I,
		KB_J = GLFW_KEY_J,
		KB_K = GLFW_KEY_K,
		KB_L = GLFW_KEY_L,
		KB_M = GLFW_KEY_M,
		KB_N = GLFW_KEY_N,
		KB_O = GLFW_KEY_O,
		KB_P = GLFW_KEY_P,
		KB_Q = GLFW_KEY_Q,
		KB_R = GLFW_KEY_R,
		KB_S = GLFW_KEY_S,
		KB_T = GLFW_KEY_T,
		KB_U = GLFW_KEY_U,
		KB_V = GLFW_KEY_V,
		KB_W = GLFW_KEY_W,
		KB_X = GLFW_KEY_X,
		KB_Y = GLFW_KEY_Y,
		KB_Z = GLFW_KEY_Z,
		KB_NUMPAD_0 = GLFW_KEY_KP_0,
		KB_NUMPAD_1 = GLFW_KEY_KP_1,
		KB_NUMPAD_2 = GLFW_KEY_KP_2,
		KB_NUMPAD_3 = GLFW_KEY_KP_3,
		KB_NUMPAD_4 = GLFW_KEY_KP_4,
		KB_NUMPAD_5 = GLFW_KEY_KP_5,
		KB_NUMPAD_6 = GLFW_KEY_KP_6,
		KB_NUMPAD_7 = GLFW_KEY_KP_7,
		KB_NUMPAD_8 = GLFW_KEY_KP_8,
		KB_NUMPAD_9 = GLFW_KEY_KP_9,// MATHS KEYS
		KB_F1 = GLFW_KEY_F1,
		KB_F2 = GLFW_KEY_F2,
		KB_F3 = GLFW_KEY_F3,
		KB_F4 = GLFW_KEY_F4,
		KB_F5 = GLFW_KEY_F5,
		KB_F6 = GLFW_KEY_F6,
		KB_F7 = GLFW_KEY_F7,
		KB_F8 = GLFW_KEY_F8,
		KB_F9 = GLFW_KEY_F9,
		KB_F10 = GLFW_KEY_F10,
		KB_F11 = GLFW_KEY_F11,
		KB_F12 = GLFW_KEY_F12,
		KB_NUMLOCK = GLFW_KEY_NUM_LOCK,
		KB_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
		KB_SHIFT_LEFT = GLFW_KEY_RIGHT_SHIFT,
	};
private:
	vector<bool> liveKBState; // Internally used, updated by window callback
	vector<bool> currentKBState;
	vector<bool> lastKBState;

	MouseData liveMouse;
	MouseData currentMouse;
	MouseData lastMouse;
public:
	Input();
	~Input();

	void Update();

	void WndProcKeyState(KB keycode, bool isDown);

	void WndProcMouseMoved(float xPos, float yPos);

	void WndProcMouseButton(int button, bool isDown);

	bool GetKey(KB keycode) const;
	bool GetKeyUp(KB keycode) const;
	bool GetKeyDown(KB keycode) const;

	DirectX::XMFLOAT2 GetMousePosition() const;
	DirectX::XMFLOAT2 GetMouseDelta() const;

	bool GetMouseButton(unsigned int button) const;
	bool GetMouseButtonUp(unsigned int button) const;
	bool GetMouseButtonDown(unsigned int button) const;
};