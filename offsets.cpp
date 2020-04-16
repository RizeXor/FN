#include "stdafx.h"

namespace Offsets {
	PVOID trampoline = 0;

	//Only update
	DWORD UWorld = 0x80A70A8;
	DWORD FnFree = 0x2AE0550;
	DWORD GetObjectName = 0x4196C20;
	DWORD GetNameByIndex = 0x2BE2B50;
	DWORD UObject = 0x7FB6780;
	DWORD GetBoneMatrix = 0x4017A70;

	DWORD CameraDecrypt = 0x660;
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
		uintptr_t base_address = (uintptr_t)GetModuleHandleW(nullptr);
		uintptr_t UObjectArray = (base_address + UObject);
		uintptr_t GetObjectNamePtr = (base_address + GetObjectName);
		uintptr_t GetNameByIndexPtr = (base_address + GetNameByIndex);
		uintptr_t FnFreePtr = (base_address + FnFree);

		/*auto addr = Utils::FindPattern(XorStr("\x48\x8B\x1D\x00\x00\x00\x00\x48\x85\xDB\x74\x3B\x41").c_str(), XorStr("xxx????xxxxxx").c_str());
		if (!addr) {
			MessageBox(0, XorStr(L"Failed to dfgdfg").c_str(), XorStr(L"gfdfgdfg").c_str(), 0);
			return FALSE;
		}*/

		bool debug = true;
		//uWorld = reinterpret_cast<decltype(uWorld)>(RELATIVE_ADDR(addr, 7));

		FortUpdater* Updater = new FortUpdater();
		if (Updater->Init(UObjectArray, GetObjectNamePtr, GetNameByIndexPtr, FnFreePtr))
		{
			Levels = Updater->FindOffset(XorStr("World").c_str(), XorStr("Levels").c_str());
			OwningGameInstance = Updater->FindOffset(XorStr("World").c_str(), XorStr("OwningGameInstance").c_str());
			
			PlayerCameraManager = Updater->FindOffset(XorStr("PlayerController").c_str(), XorStr("PlayerCameraManager").c_str());
			AcknowledgedPawn = Updater->FindOffset(XorStr("PlayerController").c_str(), XorStr("AcknowledgedPawn").c_str());
			
			LocalPlayers = Updater->FindOffset(XorStr("GameInstance").c_str(), XorStr("LocalPlayers").c_str());
			PlayerController = Updater->FindOffset(XorStr("Player").c_str(), XorStr("PlayerController").c_str());
			
			//Position shit
			RootComponent = Updater->FindOffset(XorStr("Actor").c_str(), XorStr("RootComponent").c_str());
			RelativeLocation = Updater->FindOffset(XorStr("SceneComponent").c_str(), XorStr("RelativeLocation").c_str());
			
			SkeletalMeshComponent = Updater->FindOffset(XorStr("Character").c_str(), XorStr("Mesh").c_str());
			
			//Pawn
			PlayerState = Updater->FindOffset(XorStr("Pawn").c_str(), XorStr("PlayerState").c_str());
			TeamIndex = Updater->FindOffset(XorStr("FortPlayerStateAthena").c_str(), XorStr("TeamIndex").c_str());

			if (debug)
			{
				//cout << "Uworld: 0x" << hex << uWorld << endl;
				cout << "Levels: 0x" << hex << Levels << endl;
				cout << "OwningGameInstance: 0x" << hex << OwningGameInstance << endl;
				cout << "PlayerCameraManager: 0x" << hex << PlayerCameraManager << endl;
				cout << "AcknowledgedPawn: 0x" << hex << AcknowledgedPawn << endl;
				cout << "LocalPlayers: 0x" << hex << LocalPlayers << endl;
				cout << "PlayerController: 0x" << hex << PlayerController << endl;
				cout << "RootComponent: 0x" << hex << RootComponent << endl;
				cout << "RelativeLocation: 0x" << hex << RelativeLocation << endl;
				cout << "SkeletalMeshComponent: 0x" << hex << SkeletalMeshComponent << endl;
				cout << "PlayerState: 0x" << hex << PlayerState << endl;
				cout << "TeamIndex: 0x" << hex << TeamIndex << endl;
			}
		}

		//OwningGameInstance = 0x188;

		AActors = 0x98;

		//AcknowledgedPawn = 0x298;
		//PlayerCameraManager = 0x2B0;
		//Levels = 0x148;

		//PlayerController = 0x30;

		//RootComponent = 0x130;

		//RelativeLocation = 0x11C;

		//SkeletalMeshComponent = 0x278;

		LocalPlayerPos = 0x88;
		PersistentLevel = 0x30;

		//PlayerState = 0x238;
		//TeamIndex = 0xE60;

		return TRUE;
	}
}
