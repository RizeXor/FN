#pragma once

#include <Windows.h>

namespace Offsets {
	extern PVOID* uWorld;

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

		namespace Controller {
			extern DWORD ControlRotation;
			extern PVOID SetControlRotation;
		}

		namespace PlayerController {
			extern DWORD AcknowledgedPawn;
		}

		namespace Pawn {
			extern DWORD PlayerState;
		}

		namespace PlayerState {
			extern PVOID GetPlayerName;
		}

		namespace Actor {
			extern DWORD RootComponent;
		}

		namespace Character {
			extern DWORD Mesh;
		}

		namespace SceneComponent {
			extern DWORD RelativeLocation;
			extern DWORD ComponentVelocity;
		}

		namespace StaticMeshComponent {
			extern DWORD ComponentToWorld;
			extern DWORD StaticMesh;
		}

		namespace SkinnedMeshComponent {
			extern DWORD CachedWorldSpaceBounds;
		}
	}

	BOOLEAN Initialize();
}
