#pragma once
#include <comdef.h>
#include <tchar.h>

#define D3D_CALL(x) if(FAILED(x)){\
	_com_error err(x);\
	wprintf(L"Error: in file %s at line (%d), hr: %s", _T(__FILE__), __LINE__, err.ErrorMessage());\
	DebugBreak();\
	}

#define SafeRelease(comInterface) \
if(comInterface) {comInterface->Release(); comInterface = nullptr;}