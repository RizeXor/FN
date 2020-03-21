#include "stdafx.h"

#include <Psapi.h>

FMatrix (*GetBoneMatrix)(int BoneIndex) = nullptr;

namespace Utils {
	void decrypt_name(uint64_t id, char* outbuf, int outbuf_size)
	{
		int v20 = 0;
		uint64_t game_base = 0;
		unsigned __int32 v6 = 0;
		unsigned __int64* GObjects = NULL;
		unsigned short* v12 = NULL;

		v20 = (unsigned __int16)id;

		game_base = (uint64_t)GetModuleHandleA(nullptr);

		v6 = id >> 16;

		GObjects = (unsigned __int64*)(game_base + 0x7EA8840);
		//v12 Get FNameEntry?
		v12 = (unsigned short*)(GObjects[(unsigned int)v6 + 2] + (unsigned int)(2 * v20));

		if (valid_pointer((void*)v12))
		{
			int str_size = (unsigned __int64)*v12 >> 6;

			if (str_size > outbuf_size - 1)
				return;

			memcpy(outbuf, v12 + 1, str_size);

			void* (__fastcall * decrypt_func)(void*, int) = (void* (__fastcall*)(void*, int))(game_base + 0x1FD26B0);

			decrypt_func((void*)outbuf, str_size);

			outbuf[(unsigned __int64)(unsigned __int16)*v12 >> 6] = 0;
		}
	}

	/*VOID ToMatrixWithScale(float* in, float out[4][4]) {
		auto* rotation = &in[0];
		auto* translation = &in[4];
		auto* scale = &in[8];

		out[3][0] = translation[0];
		out[3][1] = translation[1];
		out[3][2] = translation[2];

		auto x2 = rotation[0] + rotation[0];
		auto y2 = rotation[1] + rotation[1];
		auto z2 = rotation[2] + rotation[2];

		auto xx2 = rotation[0] * x2;
		auto yy2 = rotation[1] * y2;
		auto zz2 = rotation[2] * z2;
		out[0][0] = (1.0f - (yy2 + zz2)) * scale[0];
		out[1][1] = (1.0f - (xx2 + zz2)) * scale[1];
		out[2][2] = (1.0f - (xx2 + yy2)) * scale[2];

		auto yz2 = rotation[1] * z2;
		auto wx2 = rotation[3] * x2;
		out[2][1] = (yz2 - wx2) * scale[2];
		out[1][2] = (yz2 + wx2) * scale[1];

		auto xy2 = rotation[0] * y2;
		auto wz2 = rotation[3] * z2;
		out[1][0] = (xy2 - wz2) * scale[1];
		out[0][1] = (xy2 + wz2) * scale[0];

		auto xz2 = rotation[0] * z2;
		auto wy2 = rotation[3] * y2;
		out[2][0] = (xz2 + wy2) * scale[2];
		out[0][2] = (xz2 - wy2) * scale[0];

		out[0][3] = 0.0f;
		out[1][3] = 0.0f;
		out[2][3] = 0.0f;
		out[3][3] = 1.0f;
	}

	VOID MultiplyMatrices(float a[4][4], float b[4][4], float out[4][4]) {
		for (auto r = 0; r < 4; ++r) {
			for (auto c = 0; c < 4; ++c) {
				auto sum = 0.0f;

				for (auto i = 0; i < 4; ++i) {
					sum += a[r][i] * b[i][c];
				}

				out[r][c] = sum;
			}
		}
	}

	VOID GetBoneLocation(float compMatrix[4][4], PVOID bones, DWORD index, float out[3]) {
		float boneMatrix[4][4];
		ToMatrixWithScale((float*)((PBYTE)bones + (index * 0x30)), boneMatrix);

		float result[4][4];
		MultiplyMatrices(boneMatrix, compMatrix, result);

		out[0] = result[3][0];
		out[1] = result[3][1];
		out[2] = result[3][2];
	}

	float Normalize(float angle) {
		float a = (float)fmod(fmod(angle, 360.0) + 360.0, 360.0);
		if (a > 180.0f) {
			a -= 360.0f;
		}
		return a;
	}

	VOID CalcAngle(float* src, float* dst, float* angles) {
		float rel[3] = {
			dst[0] - src[0],
			dst[1] - src[1],
			dst[2] - src[2],
		};

		auto dist = sqrtf(rel[0] * rel[0] + rel[1] * rel[1] + rel[2] * rel[2]);
		auto yaw = atan2f(rel[1], rel[0]) * (180.0f / UCONST_Pi);
		auto pitch = (-((acosf((rel[2] / dist)) * 180.0f / UCONST_Pi) - 90.0f));

		angles[0] = Normalize(pitch);
		angles[1] = Normalize(yaw);
	}*/

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

	BOOLEAN Initialize()
	{
		// CreateConsole();

		ULONGLONG Base = (ULONGLONG)GetModuleHandleA(nullptr);

		auto addr = FindPattern(XorStr("\x48\x8D\x54\x24\x00\x48\x8B\xC4\x55").c_str(), XorStr("xxxx?xxxx").c_str());
		if (!addr) {
			MessageBox(0, XorStr(L"Failed to find bone matrix").c_str(), XorStr(L"dsfsdfsdf").c_str(), 0);
			return FALSE;
		}

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

	VOID CreateConsole() {
		AllocConsole();
		FILE* f;
		freopen_s(&f, "CONOUT$", "w", stdout);
	}
}
