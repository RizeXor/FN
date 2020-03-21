#include "stdafx.h"

VOID Main() {

	MH_Initialize();

	if (!Utils::Initialize())
		return;

	if (!Offsets::Initialize())
		return;

	if (!Settings::Initialize())
		return;

	if (!Render::Initialize())
		return;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		Main();
	}

	return TRUE;
}