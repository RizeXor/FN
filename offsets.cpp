#include "stdafx.h"

namespace Offsets {
	PVOID* uWorld = 0;
	DWORD FnFree = 0x2A6E690;
	DWORD GetObjectName = 0x4114180;
	DWORD CameraDecrypt = 0x660;
	DWORD UObject = 0x7EA8840;
	DWORD DecryptFunc = 0x1FD26B0;
	DWORD GetBoneMatrix = 0x3F968A0;
	DWORD ClientSetRotation = 0x628;

	namespace Engine {

		namespace World {
			DWORD OwningGameInstance = 0;
			DWORD Levels = 0;
		}

		namespace Level {
			DWORD AActors = 0;
		}

		namespace GameInstance {
			DWORD LocalPlayers = 0;
		}

		namespace Player {
			DWORD PlayerController = 0;
		}

		namespace PlayerController {
			DWORD AcknowledgedPawn = 0;
			DWORD PlayerCameraManager = 0x2B0;
		}

		namespace Pawn {
			DWORD PlayerState = 0;
			DWORD SkeletalMeshComponent = 0;
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
	}

	BOOLEAN Initialize()
	{
		auto addr = Utils::FindPattern(XorStr("\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x3B\x41").c_str(), XorStr("xxx????xxxxxx").c_str());
		if (!addr) {
			MessageBox(0, XorStr(L"Failed to find uWorld").c_str(), XorStr(L"Failure").c_str(), 0);
			return FALSE;
		}

		uWorld = reinterpret_cast<decltype(uWorld)>(RELATIVE_ADDR(addr, 7));

		Engine::World::OwningGameInstance = 0x188;

		Engine::GameInstance::LocalPlayers = 0x38;

		Engine::Level::AActors = 0x98;

		Engine::PlayerController::AcknowledgedPawn = 0x298;
		Engine::PlayerController::PlayerCameraManager = 0x2B0;

		Engine::Player::PlayerController = 0x30;

		Engine::Actor::RootComponent = 0x130;

		Engine::World::Levels = 0x148;

		Engine::SceneComponent::RelativeLocation = 0x11C;

		Engine::Pawn::SkeletalMeshComponent = 0x278;

		return TRUE;
	}
}
