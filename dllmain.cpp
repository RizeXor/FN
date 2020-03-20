#include "stdafx.h"

VOID Main() {
	MH_Initialize();
	
	if (!Render::Initialize())
		return;

	if (!Settings::Initialize())
		return;

	if (!Offsets::Initialize())
		return;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		Main();
	}

	return TRUE;
}