#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#include "d3d9framework.h"

int __stdcall WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	try {
		std::unique_ptr<orangelie::d3d9framework::d3d9app> _main_Ptr(new orangelie::d3d9framework::d3d9app);
		_main_Ptr->Initialize(960, 860, false);
		_main_Ptr->Render();

	}
	catch (const std::exception& e) {
		MessageBoxA(0, e.what(), "# Exception Error #", MB_OK);
		return -1;
	}

	return 0;
}