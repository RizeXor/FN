#include "stdafx.h"

namespace Offsets {
	PVOID* uWorld = 0;

	namespace Engine {
		namespace World {
			DWORD OwningGameInstance = 0;
			DWORD Levels = 0;
		}

		namespace Level {
			DWORD AActors = 0x98;
		}

		namespace GameInstance {
			DWORD LocalPlayers = 0;
		}

		namespace Player {
			DWORD PlayerController = 0;
		}

		namespace Controller {
			DWORD ControlRotation = 0;
			PVOID SetControlRotation = 0;
		}

		namespace PlayerController {
			DWORD AcknowledgedPawn = 0;
		}

		namespace Pawn {
			DWORD PlayerState = 0;
		}

		namespace PlayerState {
			PVOID GetPlayerName = 0;
		}

		namespace Actor {
			DWORD RootComponent = 0;
		}

		namespace Character {
			DWORD Mesh = 0;
		}

		namespace SceneComponent {
			DWORD RelativeLocation = 0;
			DWORD ComponentVelocity = 0;
		}

		namespace StaticMeshComponent {
			DWORD ComponentToWorld = 0x1C0;
			DWORD StaticMesh = 0;
		}

		namespace SkinnedMeshComponent {
			DWORD CachedWorldSpaceBounds = 0;
		}
	}

	BOOLEAN Initialize()
	{
		auto addr = Utils::FindPattern(XorStr("\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x3B\x41").c_str(), XorStr("xxx????xxxxxx").c_str());
		if (!addr) {
			MessageBox(0, XorStr(L"Failed to find uWorld").c_str(), XorStr(L"Failure").c_str(), 0);
			return FALSE;
		}
		uWorld = reinterpret_cast<decltype(uWorld)>(RELATIVE_ADDR(addr, 7));

		return TRUE;
	}
}
