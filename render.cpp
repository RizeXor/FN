#include "stdafx.h"
#include "hooks.h"
#include "imgui/imgui_impl_dx11.h"

IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static int player_list_size = 0;
uintptr_t UworldAddress = 0;
int Pawns = 0;
char* PlayerName = (char*)"Nothing";
int pwnId = 0;
static float width = 0;
static float height = 0;
static HWND hWnd = 0;
float Pitch = 0;
FMinimalViewInfo myinfo;
bool pressed = false;
PVOID trampoline = 0;
ULONGLONG GetObjectNameAddr = 0;
void(__fastcall * csr_func)(uint64_t, FRotator, bool) = nullptr;

namespace Render {
	BOOLEAN showMenu = FALSE;

	WNDPROC WndProcOriginal = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* immediateContext = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;

	HRESULT(*PresentOriginal)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr;
	HRESULT(*ResizeOriginal)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;

	ImGuiWindow& BeginScene() {
		ImGui_ImplDX11_NewFrame();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin(XorStr("##scene").c_str(), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

		auto& io = ImGui::GetIO();
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

		return *ImGui::GetCurrentWindow();
	}

	VOID EndScene(ImGuiWindow& window) {
		window.DrawList->PushClipRectFullScreen();
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.17f, 0.18f, 0.2f, 1.0f));

		if (showMenu) {
			ImGui::Begin(XorStr("##menu").c_str(), reinterpret_cast<bool*>(true), ImGuiWindowFlags_NoCollapse);
			ImGui::SetWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
			ImGui::Text(XorStr("GayNiteFN").c_str());

			ImGui::Text(XorStr("Aimbot:").c_str());
			ImGui::Checkbox(XorStr("Aimbot").c_str(), &settings.Aimbot);

			ImGui::Text(XorStr("Visuals:").c_str());
			ImGui::Checkbox(XorStr("Player Lines").c_str(), &settings.ESP.PlayerLines);
			ImGui::Checkbox(XorStr("Bone ESP").c_str(), &settings.ESP.BoneESP);
			ImGui::Checkbox(XorStr("Box ESP").c_str(), &settings.ESP.BOXESP);
			ImGui::Checkbox(XorStr("Aimbot FOV").c_str(), &settings.FOV);
			ImGui::Checkbox(XorStr("Players Around").c_str(), &settings.PlayersAround);
			if (settings.FOV) {
				ImGui::SliderFloat(XorStr("aim-fov##slider").c_str(), &settings.FOVSize, 0.0f, 1000.0f, XorStr("%.2f").c_str());
			}

			ImGui::Text(XorStr("Debug:").c_str());
			if (!settings.Debug) {
				ImGui::Text(XorStr("Pitch -> %f\n").c_str(), Pitch);
				ImGui::Text(XorStr("Pawns -> %lu\n").c_str(), Pawns);
				ImGui::Text(XorStr("tramp -> 0x%llx\n").c_str(), trampoline);
				char buffer[40] = { 0 };
				sprintf_s(buffer, "%llx\n", UworldAddress);
				ImGui::InputText(XorStr("UWorld").c_str(), buffer, 40);
			}

			ImGui::End();
		}

		ImGui::PopStyleColor();

		ImGui::Render();
	}

	LRESULT CALLBACK WndProcHook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (msg == WM_KEYUP && (wParam == VK_INSERT || (showMenu && wParam == VK_ESCAPE))) {
			showMenu = !showMenu;
			ImGui::GetIO().MouseDrawCursor = showMenu;
		}
		else if (msg == WM_QUIT && showMenu) {
			ExitProcess(0);
		}

		if (showMenu) {
			ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
			return TRUE;
		}

		return CallWindowProc(WndProcOriginal, hwnd, msg, wParam, lParam);
	}

	VOID AddLine(ImGuiWindow& window, float width, float height, FVector a, FVector b, ImU32 color, float& minX, float& maxX, float& minY, float& maxY) {
		FVector2D ac = Utils::WorldToScreen(a, myinfo);
		FVector2D bc = Utils::WorldToScreen(b, myinfo);
		if (true) {
			window.DrawList->AddLine(ImVec2(ac.x, ac.y), ImVec2(bc.x, bc.y), color, 1.2f);

			minX = min(ac.x, minX);
			minX = min(bc.x, minX);

			maxX = max(ac.x, maxX);
			maxX = max(bc.x, maxX);

			minY = min(ac.y, minY);
			minY = min(bc.y, minY);

			maxY = max(ac.y, maxY);
			maxY = max(bc.y, maxY);
		}
	}

	void InitClientSetRotation(ULONGLONG PlayerController) {
		auto PlayerControllerVTable = *(ULONGLONG*)(PlayerController);
		if (!valid_pointer((void*)PlayerControllerVTable)) return;

		csr_func =
			(*(void(__fastcall**)(uint64_t, FRotator, bool))(PlayerControllerVTable + Offsets::ClientSetRotation));
	}

	void InitAddresses() {
		PVOID Base = (PVOID)GetModuleHandle(nullptr);
		//41 ff 26
		//trampoline = (PVOID)((ULONGLONG)Base + 0x22702c0);
		trampoline = (PVOID)Utils::FindPattern("\x41\xFF\x26", "xxx");
		GetObjectNameAddr = ((ULONGLONG)Base + Offsets::GetObjectName);
	}

	void DecryptCamera(ULONGLONG PlayerCameraManager) {
		auto PlayerCameraManagerVTable = *(ULONGLONG*)(PlayerCameraManager);
		if (!valid_pointer((void*)PlayerCameraManagerVTable)) return;

		uint64_t(__fastcall * func)(uint64_t, FMinimalViewInfo*) =
			(*(uint64_t(__fastcall*)(uint64_t, FMinimalViewInfo*))(*(uint64_t*)(PlayerCameraManagerVTable + Offsets::CameraDecrypt)));
		Utils::spoof_call(trampoline, func, (uint64_t)PlayerCameraManager, &myinfo);
		Pitch = myinfo.Rotation.pitch;
	}

	FString GetObjName(UINT64* obj)
	{
		return reinterpret_cast<FString(__fastcall*)(UINT64*)>(GetObjectNameAddr)(obj);
	}

	HRESULT PresentHook(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
		if (!device) {
			swapChain->GetDevice(__uuidof(device), reinterpret_cast<PVOID*>(&device));
			device->GetImmediateContext(&immediateContext);

			ID3D11Texture2D* renderTarget = nullptr;
			swapChain->GetBuffer(0, __uuidof(renderTarget), reinterpret_cast<PVOID*>(&renderTarget));
			device->CreateRenderTargetView(renderTarget, nullptr, &renderTargetView);
			renderTarget->Release();

			ID3D11Texture2D* backBuffer = 0;
			swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
			D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
			backBuffer->GetDesc(&backBufferDesc);

			hWnd = FindWindow(XorStr(L"UnrealWindow").c_str(), XorStr(L"Fortnite  ").c_str());
			if (!width) {
				WndProcOriginal = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProcHook)));
			}

			width = (float)backBufferDesc.Width;
			height = (float)backBufferDesc.Height;
			backBuffer->Release();

			HWND targetWindow = 0;
			EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
				DWORD pid = 0;
				GetWindowThreadProcessId(hWnd, &pid);
				if (pid == GetCurrentProcessId()) {
					*reinterpret_cast<HWND*>(lParam) = hWnd;
					return FALSE;
				}

				return TRUE;
			}, reinterpret_cast<LPARAM>(&targetWindow));

			ImGui_ImplDX11_Init(targetWindow, device, immediateContext);
			ImGui_ImplDX11_CreateDeviceObjects();

			InitAddresses();
		}

		immediateContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

		auto& window = BeginScene();

		if (settings.FOV) {
			window.DrawList->AddCircle(ImVec2(960, height / 2), settings.FOVSize, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, 1.0f }), 32, 2.0f);
		}

		auto success = FALSE;
		float closestDistance = FLT_MAX;
		ULONGLONG closestPawn = NULL;
		ULONGLONG targetPawn = NULL;
		ULONGLONG PlayerController = 0;
		do {
			auto world = *Offsets::uWorld;
			if (!valid_pointer((void*)world)) break;

			UworldAddress = (ULONGLONG)world;

			ULONGLONG PersistentLevel = *(ULONGLONG*)((ULONGLONG)world + Offsets::PersistentLevel);
			if (!valid_pointer((void*)PersistentLevel)) break;

			int ActorCount = *(UINT32*)(PersistentLevel + Offsets::AActors + sizeof(ULONGLONG));
			if (ActorCount == 0) break;

			auto gameInstance = ReadPtr((ULONGLONG)world, Offsets::OwningGameInstance);
			if (!valid_pointer((void*)gameInstance)) break;

			auto localPlayers = ReadPtr(gameInstance, Offsets::LocalPlayers);
			if (!valid_pointer((void*)localPlayers)) break;

			auto localPlayer = ReadPtr(localPlayers, 0);
			if (!valid_pointer((void*)localPlayer)) break;

			FVector LocalPlayerPos = *(FVector*)(localPlayers + Offsets::LocalPlayerPos);

			PlayerController = ReadPtr(localPlayer, Offsets::PlayerController);
			if (!valid_pointer((void*)PlayerController)) break;

			InitClientSetRotation(PlayerController);

			auto PlayerCameraManager = ReadPtr(PlayerController, Offsets::PlayerCameraManager);
			if (!valid_pointer((void*)PlayerCameraManager)) break;

			DecryptCamera(PlayerCameraManager);

			auto AcknowledgedPawn = ReadPtr(PlayerController, Offsets::AcknowledgedPawn);

			if (!valid_pointer(AcknowledgedPawn)) break;

			auto LocalPlayerState = ReadPtr(AcknowledgedPawn, Offsets::PlayerState);

			if (!valid_pointer(LocalPlayerState)) break;

			auto TeamIndex = *(int*)(LocalPlayerState + Offsets::TeamIndex);

			Pawns = 0;
			auto ActorList = ReadPtr(PersistentLevel, Offsets::AActors);

			if (!valid_pointer((void*)ActorList)) break;

			for (unsigned int pw = 0; pw < ActorCount; pw++) {
				auto Actor = ReadPtr(ActorList, pw * sizeof(ULONGLONG));

				if (!valid_pointer((void*)Actor) || Actor == AcknowledgedPawn)
					continue;

				auto ObjectName = GetObjName((ULONGLONG*)Actor);
				char* buffer = (char*)ObjectName.ToString().c_str();

				if (strstr(buffer, XorStr("PlayerPawn_Athena_C").c_str()) ||
					strstr(buffer, XorStr("PlayerPawn_Athena_Phoebe_C").c_str()) ||
					strstr(buffer, XorStr("BP_MangPlayerPawn_Default_C").c_str()) ||
					strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_Midas_C").c_str()) ||
					strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_Meowscles_C").c_str()) ||
					strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_AdventureGirl_C").c_str()) ||
					strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_TnTina_C").c_str()) ||
					strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_HeadOfSecurity_C").c_str())) {
					bool bot = false;

					if (strstr(buffer, XorStr("PlayerPawn_Athena_Phoebe_C").c_str()) ||
						strstr(buffer, XorStr("BP_MangPlayerPawn_Default_C").c_str()) ||
						strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_Midas_C").c_str()) ||
						strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_Meowscles_C").c_str()) ||
						strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_AdventureGirl_C").c_str()) ||
						strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_TnTina_C").c_str()) ||
						strstr(buffer, XorStr("BP_MangPlayerPawn_Boss_HeadOfSecurity_C").c_str())) {
						bot = true;
					}

					auto EnemyTeamState = ReadPtr(Actor, Offsets::PlayerState);
					if (!valid_pointer(EnemyTeamState)) continue;

					auto ActorTeamIndex = *(int*)(EnemyTeamState + Offsets::TeamIndex);

					FVector neck;
					if (!Utils::GetBoneMatrix(Actor, 65, &neck))
						continue;

					float minX = FLT_MAX;
					float maxX = -FLT_MAX;
					float minY = FLT_MAX;
					float maxY = -FLT_MAX;

					auto color = ImGui::GetColorU32({ 0.0f, 0.0f, 1.0f, 1.0f });
					if(TeamIndex == ActorTeamIndex)
						color = ImGui::GetColorU32({ 0.0f, 1.0f, 0.0f, 1.0f });
					else if(!bot)
						color = ImGui::GetColorU32({ 1.0f, 0.0f, 0.0f, 1.0f });

					FVector2D worldPawnPos = Utils::WorldToScreen(neck, myinfo);

					if (settings.ESP.BoneESP) {
						//Top
						FVector head;
						if (!Utils::GetBoneMatrix(Actor, 66, &head))
							continue;

						FVector chest;
						if (!Utils::GetBoneMatrix(Actor, 36, &chest))
							continue;

						FVector pelvis;
						if (!Utils::GetBoneMatrix(Actor, 2, &pelvis))
							continue;

						//Arms
						FVector leftShoulder;
						if (!Utils::GetBoneMatrix(Actor, 9, &leftShoulder))
							continue;

						FVector rightShoulder;
						if (!Utils::GetBoneMatrix(Actor, 62, &rightShoulder))
							continue;

						FVector leftElbow;
						if (!Utils::GetBoneMatrix(Actor, 10, &leftElbow))
							continue;

						FVector rightElbow;
						if (!Utils::GetBoneMatrix(Actor, 38, &rightElbow))
							continue;

						FVector leftHand;
						if (!Utils::GetBoneMatrix(Actor, 11, &leftHand))
							continue;

						FVector rightHand;
						if (!Utils::GetBoneMatrix(Actor, 39, &rightHand))
							continue;

						//Bottom
						FVector leftLeg;
						if (!Utils::GetBoneMatrix(Actor, 67, &leftLeg))
							continue;

						FVector rightLeg;
						if (!Utils::GetBoneMatrix(Actor, 74, &rightLeg))
							continue;

						FVector leftThigh;
						if (!Utils::GetBoneMatrix(Actor, 73, &leftThigh))
							continue;

						FVector rightThigh;
						if (!Utils::GetBoneMatrix(Actor, 80, &rightThigh))
							continue;

						FVector leftFoot;
						if (!Utils::GetBoneMatrix(Actor, 68, &leftFoot))
							continue;

						FVector rightFoot;
						if (!Utils::GetBoneMatrix(Actor, 75, &rightFoot))
							continue;

						FVector leftFeet;
						if (!Utils::GetBoneMatrix(Actor, 71, &leftFeet))
							continue;

						FVector rightFeet;
						if (!Utils::GetBoneMatrix(Actor, 78, &rightFeet))
							continue;

						FVector leftFeetFinger;
						if (!Utils::GetBoneMatrix(Actor, 72, &leftFeetFinger))
							continue;

						FVector rightFeetFinger;
						if (!Utils::GetBoneMatrix(Actor, 79, &rightFeetFinger))
							continue;

						AddLine(window, width, height, head, neck, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, neck, pelvis, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, chest, leftShoulder, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, chest, rightShoulder, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftShoulder, leftElbow, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightShoulder, rightElbow, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftElbow, leftHand, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightElbow, rightHand, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, pelvis, leftLeg, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, pelvis, rightLeg, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftLeg, leftThigh, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightLeg, rightThigh, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftThigh, leftFoot, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightThigh, rightFoot, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftFoot, leftFeet, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightFoot, rightFeet, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, leftFeet, leftFeetFinger, color, minX, maxX, minY, maxY);
						AddLine(window, width, height, rightFeet, rightFeetFinger, color, minX, maxX, minY, maxY);
					}

					if (minX < width && maxX > 0 && minY < height && maxY > 0 && settings.ESP.BOXESP) {
						auto topLeft = ImVec2(minX - 3.0f, minY - 3.0f);
						auto bottomRight = ImVec2(maxX + 3.0f, maxY + 3.0f);

						window.DrawList->AddRectFilled(topLeft, bottomRight, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, 0.10f }));
						window.DrawList->AddRect(topLeft, bottomRight, ImGui::GetColorU32({ 0.0f, 0.50f, 0.90f, 1.0f }), 0.5, 15, 1.5f);
					}

					if (settings.ESP.PlayerLines) {
						window.DrawList->AddLine(ImVec2(960, height),
							ImVec2(worldPawnPos.x, worldPawnPos.y), ImGui::GetColorU32({ 1.0f, 1.0f, 1.0f, 1.0f }));
					}

					if (TeamIndex == ActorTeamIndex) {
						Pawns++;
						continue;
					}

					if (settings.Aimbot) {
						float dx = worldPawnPos.x - 960.0f;
						float dy = worldPawnPos.y - 540.0f;
						auto dist = sqrtf(dx * dx + dy * dy);
						if (dist < settings.FOVSize && dist < closestDistance) {
							closestDistance = dist;
							closestPawn = Actor;
						}
					}

					Pawns++;
				}
				else {
					continue;
				}
			}

			if (settings.Aimbot && closestPawn && GetAsyncKeyState(VK_RBUTTON) < 0 && GetForegroundWindow() == hWnd && !pressed) {
				targetPawn = closestPawn;
				pressed = true;
			}
			else {
				targetPawn = NULL;
				pressed = false;
			}

			if (settings.PlayersAround) {
				char EnemiesBuffer[32];
				sprintf_s(EnemiesBuffer, XorStr("Enemies: %u %d %f").c_str(), Pawns, pressed, myinfo.Rotation.pitch);
				window.DrawList->AddText(ImVec2(960, 100), ImGui::GetColorU32({ 1.0f, 0.0f, 0.0f, 1.0f }), EnemiesBuffer);
			}

			success = TRUE;
		} while (FALSE);

		if (!success) {
			targetPawn = NULL;
		}

		if (settings.Aimbot && success && targetPawn && valid_pointer(PlayerController) && pressed && Pawns) {
			FRotator angles;
			if(Utils::get_aim_angles(myinfo.Rotation, myinfo.Location, targetPawn, 65, &angles))
				Utils::spoof_call(trampoline, csr_func, PlayerController, angles, false);
		}

		Render::EndScene(window);

		return PresentOriginal(swapChain, syncInterval, flags);
	}

	__declspec(dllexport) HRESULT ResizeHook(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
		ImGui_ImplDX11_Shutdown();
		renderTargetView->Release();
		immediateContext->Release();
		device->Release();
		device = nullptr;

		return ResizeOriginal(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
	}

	BOOLEAN Initialize() {
		IDXGISwapChain* swapChain = nullptr;
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		auto                 featureLevel = D3D_FEATURE_LEVEL_11_0;

		DXGI_SWAP_CHAIN_DESC sd = { 0 };
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = FindWindowA(XorStr("UnrealWindow").c_str(), XorStr("Fortnite  ").c_str());
		sd.SampleDesc.Count = 1;
		sd.Windowed = TRUE;

		if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context))) {
			MessageBox(0, XorStr(L"Failed to create D3D11 device and swap chain").c_str(), XorStr(L"Failure").c_str(), MB_ICONERROR);
			return FALSE;
		}

		auto table = *reinterpret_cast<PVOID**>(swapChain);
		auto present = table[8];
		auto resize = table[13];

		context->Release();
		device->Release();
		swapChain->Release();

		MH_CreateHook(present, PresentHook, reinterpret_cast<PVOID*>(&PresentOriginal));
		MH_EnableHook(present);

		MH_CreateHook(resize, ResizeHook, reinterpret_cast<PVOID*>(&ResizeOriginal));
		MH_EnableHook(resize);

		MH_CreateHook(&GetWindowTextA, GetWindowTextAHook, reinterpret_cast<PVOID*>(&GetWindowTextAOriginal));
		MH_EnableHook(&GetWindowTextA);

		MH_CreateHook(&GetWindowTextW, GetWindowTextWHook, reinterpret_cast<PVOID*>(&GetWindowTextWOriginal));
		MH_EnableHook(&GetWindowTextW);

		MH_CreateHook(&CreateFileA, CreateFileAHook, reinterpret_cast<PVOID*>(&CreateFileAOriginal));
		MH_EnableHook(&CreateFileA);

		MH_CreateHook(&CreateFileW, CreateFileWHook, reinterpret_cast<PVOID*>(&CreateFileWOriginal));
		MH_EnableHook(&CreateFileW);

		MH_CreateHook(&DeviceIoControl, DeviceIoControlHook, reinterpret_cast<PVOID*>(&DeviceIoControlOriginal));
		MH_EnableHook(&DeviceIoControl);

		MH_CreateHook(&SetupDiGetDevicePropertyW, SetupDiGetDevicePropertyWHook, reinterpret_cast<PVOID*>(&SetupDiGetDevicePropertyWOriginal));
		MH_EnableHook(&SetupDiGetDevicePropertyW);

		MH_CreateHook(&SetupDiGetDeviceRegistryPropertyW, SetupDiGetDeviceRegistryPropertyWHook, reinterpret_cast<PVOID*>(&SetupDiGetDeviceRegistryPropertyWOriginal));
		MH_EnableHook(&SetupDiGetDeviceRegistryPropertyW);

		MH_CreateHook(&SetupDiGetDeviceInstanceIdW, SetupDiGetDeviceInstanceIdWHook, reinterpret_cast<PVOID*>(&SetupDiGetDeviceInstanceIdWOrignal));
		MH_EnableHook(&SetupDiGetDeviceInstanceIdW);

		return TRUE;
	}
}
