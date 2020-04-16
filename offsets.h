#pragma once

#include <Windows.h>

namespace Offsets {
	extern PVOID trampoline;

	extern DWORD UWorld;
	extern DWORD FnFree;
	extern DWORD GetObjectName;
	extern DWORD CameraDecrypt;
	extern DWORD UObject;
	extern DWORD GetBoneMatrix;
	extern DWORD GetNameByIndex;
	extern DWORD ClientSetRotation;

	extern DWORD OwningGameInstance;
	extern DWORD LocalPlayers;
	extern DWORD AActors;
	extern DWORD AcknowledgedPawn;
	extern DWORD PlayerCameraManager;
	extern DWORD PlayerController;
	extern DWORD RootComponent;
	extern DWORD Levels;
	extern DWORD RelativeLocation;
	extern DWORD SkeletalMeshComponent;
	extern DWORD LocalPlayerPos;
	extern DWORD PersistentLevel;
	extern DWORD PlayerState;
	extern DWORD TeamIndex;

	BOOLEAN Initialize();
}
