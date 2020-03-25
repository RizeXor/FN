#include "stdafx.h"

#include <Psapi.h>

ULONGLONG GetBoneMatrixAddress = 0;

namespace Utils {
	bool get_aim_angles(FRotator cam_rotation, FVector cam_location, uint64_t aactor, int bone, FRotator* out)
	{
		FVector vec;
		float x = 0, y = 0, z = 0;
		float distance = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		FVector pos;

		if (!valid_pointer((void*)aactor))
		{
			return false;
		}

		if (!GetBoneMatrix(aactor, bone, &vec))
		{
			return false;
		}

		x = abs(cam_location.x - vec.x);
		y = abs(cam_location.y - vec.y);
		z = abs(cam_location.z - vec.z);

		distance = sqrtf(x * x +  y * y + z * z);

		pos.x = vec.x - cam_location.x;
		pos.y = vec.y - cam_location.y;
		pos.z = vec.z - cam_location.z;

		pitch = -((acos(pos.z / distance) * 180.0f / (float)M_PI) - 90.0f);
		yaw = (atan2(pos.y, pos.x) * 180.0f) / (float)M_PI;

		out->pitch = cam_rotation.pitch + (pitch - cam_rotation.pitch);
		out->yaw = cam_rotation.yaw + (yaw - cam_rotation.yaw);
		out->roll = 0.0f;

		return true;
	}

	D3DMATRIX GetMatrix(FRotator rot, FVector origin = FVector(0, 0, 0))
	{
		float radPitch = (rot.pitch * float(M_PI) / 180.f);
		float radYaw = (rot.yaw * float(M_PI) / 180.f);
		float radRoll = (rot.roll * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}

	FVector2D WorldToScreen(FVector WorldLocation, FMinimalViewInfo info)
	{
		FVector2D Screenlocation = FVector2D(0, 0);

		FRotator Rotation = info.Rotation; // FRotator

		D3DMATRIX tempMatrix = GetMatrix(Rotation); // Matrix

		FVector vAxisX, vAxisY, vAxisZ;

		vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		FVector vDelta = WorldLocation - info.Location;
		FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;

		float FovAngle = info.FOV;
		float ScreenCenterX = 960.0f;
		float ScreenCenterY = 540.0f;

		Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
		Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

		return Screenlocation;
	}

	bool GetBoneMatrix(ULONGLONG Actor, unsigned int index, FVector* Out) {
		ULONGLONG ActorMesh = 0;
		FMatrix matrix;
		FMatrix* tmp = nullptr;

		ActorMesh = *(ULONGLONG*)(Actor + Offsets::SkeletalMeshComponent);
		if (!valid_pointer((void*)ActorMesh)) {
			return false;
		}

		FMatrix* (__fastcall * func)(void*, void*, int) = (FMatrix * (__fastcall *)(void*, void*, int))GetBoneMatrixAddress;
		tmp = func((void*)ActorMesh, (void*)&matrix, index);

		Out->x = matrix.WPlane.x;
		Out->y = matrix.WPlane.y;
		Out->z = matrix.WPlane.z;

		return true;
	}

	BOOLEAN Initialize()
	{
		ULONGLONG Base = (ULONGLONG)GetModuleHandleA(nullptr);
		GetBoneMatrixAddress = (Base + Offsets::GetBoneMatrix);

		/*GetBoneMatrixAddress = (ULONGLONG)FindPattern(
			XorStr("\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x56\x41\x57\x48\x8D\x68\xA1\x48\x81\xEC\xE0\x00\x00\x00\x0F\x29\x78\xB8\x33\xF6").c_str(), 
			XorStr("xxxxxxxxxxxxxxxxxxxxx???xxxxxx").c_str());*/

		/*if (!GetBoneMatrixAddress) {
			MessageBox(0, XorStr(L"Failed to find bone matrix").c_str(), XorStr(L"dsfsdfsdf").c_str(), 0);
			return FALSE;
		}*/

		return TRUE;
	}

	BOOLEAN MaskCompare(PVOID buffer, LPCSTR pattern, LPCSTR mask) {
		for (auto b = reinterpret_cast<PBYTE>(buffer); *mask; ++pattern, ++mask, ++b) {
			if (*mask == 'x' && *reinterpret_cast<LPCBYTE>(pattern) != *b) {
				return FALSE;
			}
		}

		return TRUE;
	}

	PBYTE FindPattern(PVOID base, DWORD size, LPCSTR pattern, LPCSTR mask) {
		size -= static_cast<DWORD>(strlen(mask));

		for (auto i = 0UL; i < size; ++i) {
			auto addr = reinterpret_cast<PBYTE>(base) + i;
			if (MaskCompare(addr, pattern, mask)) {
				return addr;
			}
		}

		return NULL;
	}

	PBYTE FindPattern(LPCSTR pattern, LPCSTR mask) {
		MODULEINFO info = { 0 };
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));

		return FindPattern(info.lpBaseOfDll, info.SizeOfImage, pattern, mask);
	}
}
