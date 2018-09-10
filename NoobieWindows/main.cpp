#include "NoobieApp.h"

//https://github.com/Microsoft/FX11/wiki/Effects-11
//https://blogs.msdn.microsoft.com/chuckw/2013/08/20/living-without-d3dx/
//https://blogs.msdn.microsoft.com/chuckw/2012/10/23/effects-for-direct3d-11-update/

int main()
{
	NoobieApp app(L"Noobie Window", 1280, 720);

	if (!app.Init())
		return 0;

	app.Run();

	return 0;
}