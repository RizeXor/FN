#pragma once

#include <Windows.h>

namespace Offsets {
	extern PVOID* uWorld;
	extern DWORD FnFree;
	extern DWORD GetObjectName;
	extern DWORD CameraDecrypt;
	extern DWORD UObject;
	extern DWORD DecryptFunc;
	extern DWORD GetBoneMatrix;

	namespace Engine {

		namespace World {
			extern DWORD OwningGameInstance;
			extern DWORD Levels;
		}

		namespace Level {
			extern DWORD AActors;
		}

		namespace GameInstance {
			extern DWORD LocalPlayers;
		}

		namespace Player {
			extern DWORD PlayerController;
		}

		namespace PlayerController {
			extern DWORD AcknowledgedPawn;
			extern DWORD PlayerCameraManager;
		}

		namespace Pawn {
			extern DWORD PlayerState;
			extern DWORD SkeletalMeshComponent;
		}

		namespace PlayerState {
			extern PVOID GetPlayerName;
		}

		namespace Actor {
			extern DWORD RootComponent;
		}

		namespace SceneComponent {
			extern DWORD RelativeLocation;
			extern DWORD ComponentVelocity;
		}
	}

	BOOLEAN Initialize();
}
