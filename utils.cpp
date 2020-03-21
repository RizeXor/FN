#include "stdafx.h"

#include <Psapi.h>

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
			//void* ret = NULL;

			if (str_size > outbuf_size - 1)
				return;

			memcpy(outbuf, v12 + 1, str_size);

			void* (__fastcall * decrypt_func)(void*, int) = (void* (__fastcall*)(void*, int))(game_base + 0x1FD26B0);

			////ret = decrypt_func((void*)outbuf, str_size);
			decrypt_func((void*)outbuf, str_size);

			outbuf[(unsigned __int64)(unsigned __int16)*v12 >> 6] = 0;
		}
	}

	BOOLEAN Initialize()
	{
		// CreateConsole();

		ULONGLONG Base = (ULONGLONG)GetModuleHandleA(nullptr);
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
