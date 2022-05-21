/*
*
* < main.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#include "SAMPLES/GridAndCameraMovement.h"

int __stdcall WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow) {


#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	

	try {
		std::unique_ptr<orangelie::Engine::ZekrosEngine> zekrosEngine(new GridAndCameraMovement);
		zekrosEngine->Initialize(920, 860, false);
		zekrosEngine->Run();
	}
	catch (const std::exception& e) {
		MessageBoxA(0, e.what(), "@@ Exception Erorr ! @@", MB_ICONWARNING);
		return -1;
	}

	return 0;
}