#include "stdafx.h"

namespace Offsets {
	PVOID* uWorld = 0;
	DWORD FnFree = 0x2a6f280;
	DWORD GetObjectName = 0x4115060;
	DWORD CameraDecrypt = 0x660;
	DWORD UObject = 0x7ec2110;
	DWORD DecryptFunc = 0x1FD26B0;
	DWORD GetBoneMatrix = 0x3F97490;
	DWORD ClientSetRotation = 0x628;
	
	DWORD OwningGameInstance = 0;
	DWORD LocalPlayers = 0;
	DWORD AActors = 0;
	DWORD AcknowledgedPawn = 0;
	DWORD PlayerCameraManager = 0;
	DWORD PlayerController = 0;
	DWORD RootComponent = 0;
	DWORD Levels = 0;
	DWORD RelativeLocation = 0;
	DWORD SkeletalMeshComponent = 0;
	DWORD LocalPlayerPos = 0;
	DWORD PersistentLevel = 0;
	DWORD PlayerState = 0;
	DWORD TeamIndex = 0;

	BOOLEAN Initialize()
	{
		auto addr = Utils::FindPattern(XorStr("\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x3B\x41").c_str(), XorStr("xxx????xxxxxx").c_str());
		if (!addr) {
			MessageBox(0, XorStr(L"Failed to dfgdfg").c_str(), XorStr(L"gfdfgdfg").c_str(), 0);
			return FALSE;
		}

		uWorld = reinterpret_cast<decltype(uWorld)>(RELATIVE_ADDR(addr, 7));

		OwningGameInstance = 0x188;

		LocalPlayers = 0x38;

		AActors = 0x98;

		AcknowledgedPawn = 0x298;
		PlayerCameraManager = 0x2B0;

		PlayerController = 0x30;

		RootComponent = 0x130;

		Levels = 0x148;

		RelativeLocation = 0x11C;

		SkeletalMeshComponent = 0x278;

		LocalPlayerPos = 0x88;
		PersistentLevel = 0x30;

		PlayerState = 0x238;
		TeamIndex = 0x260;

		return TRUE;
	}
}
