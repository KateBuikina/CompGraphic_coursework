#include "MyGraphics.h"

int main()
{
	MyGraphics window;
	
	if (!window.ConstructConsole(360, 200, 2, 2, L"CW_CG_23"))
		window.Loop();

	return 0;
}