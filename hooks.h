#pragma once

#include <Windows.h>
#include <SetupAPI.h>
#include <string>
#include <codecvt>

int (*GetWindowTextAOriginal)(HWND hWnd, LPSTR lpString, int nMaxCount) = nullptr;
int (*GetWindowTextWOriginal)(HWND hWnd, LPWSTR lpString, int nMaxCount) = nullptr;
HANDLE(*CreateFileAOriginal)(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	) = nullptr;

HANDLE(*CreateFileWOriginal)(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	) = nullptr;
BOOL(*DeviceIoControlOriginal)(
	HANDLE       hDevice,
	DWORD        dwIoControlCode,
	LPVOID       lpInBuffer,
	DWORD        nInBufferSize,
	LPVOID       lpOutBuffer,
	DWORD        nOutBufferSize,
	LPDWORD      lpBytesReturned,
	LPOVERLAPPED lpOverlapped
	) = nullptr;

BOOL(*SetupDiGetDevicePropertyWOriginal)(
	HDEVINFO         DeviceInfoSet,
	PSP_DEVINFO_DATA DeviceInfoData,
	const DEVPROPKEY* PropertyKey,
	DEVPROPTYPE* PropertyType,
	PBYTE            PropertyBuffer,
	DWORD            PropertyBufferSize,
	PDWORD           RequiredSize,
	DWORD            Flags
	) = nullptr;

BOOL(*SetupDiGetDeviceRegistryPropertyWOriginal)(
	HDEVINFO         DeviceInfoSet,
	PSP_DEVINFO_DATA DeviceInfoData,
	DWORD            Property,
	PDWORD           PropertyRegDataType,
	PBYTE            PropertyBuffer,
	DWORD            PropertyBufferSize,
	PDWORD           RequiredSize
	) = nullptr;

BOOL(*SetupDiGetDeviceInstanceIdWOrignal)(
	HDEVINFO         DeviceInfoSet,
	PSP_DEVINFO_DATA DeviceInfoData,
	PWSTR            DeviceInstanceId,
	DWORD            DeviceInstanceIdSize,
	PDWORD           RequiredSize
	) = nullptr;

int GetWindowTextAHook(HWND hWnd, LPSTR lpString, int nMaxCount) {
	return 0;
}

int GetWindowTextWHook(HWND hWnd, LPWSTR lpString, int nMaxCount) {
	return 0;
}

HANDLE CreateFileAHook(
	LPCSTR                file_name,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	)
{
	if (strstr(file_name, "BattlEye") || strstr(file_name, "EasyAntiCheat"))
	{
		return CreateFileAOriginal(file_name, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	return INVALID_HANDLE_VALUE;
}

HANDLE CreateFileWHook(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::wstring wide;
	std::string str;

	wide = lpFileName;

	str = conv.to_bytes(wide);

	if (strstr(str.c_str(), "Fortnite"))
	{
		return CreateFileWOriginal(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	return NULL;
}

BOOL DeviceIoControlHook(
	HANDLE       hDevice,
	DWORD        dwIoControlCode,
	LPVOID       lpInBuffer,
	DWORD        nInBufferSize,
	LPVOID       lpOutBuffer,
	DWORD        nOutBufferSize,
	LPDWORD      lpBytesReturned,
	LPOVERLAPPED lpOverlapped
	)
{
	if (dwIoControlCode == IOCTL_STORAGE_QUERY_PROPERTY)
	{
		return 0;
	}

	return DeviceIoControlOriginal(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
}

BOOL SetupDiGetDevicePropertyWHook(
	HDEVINFO         DeviceInfoSet,
	PSP_DEVINFO_DATA DeviceInfoData,
	const DEVPROPKEY* PropertyKey,
	DEVPROPTYPE* PropertyType,
	PBYTE            PropertyBuffer,
	DWORD            PropertyBufferSize,
	PDWORD           RequiredSize,
	DWORD            Flags
	)
{
	return 0;
}

BOOL SetupDiGetDeviceRegistryPropertyWHook(
	HDEVINFO         DeviceInfoSet,
	PSP_DEVINFO_DATA DeviceInfoData,
	DWORD            Property,
	PDWORD           PropertyRegDataType,
	PBYTE            PropertyBuffer,
	DWORD            PropertyBufferSize,
	PDWORD           RequiredSize
	)
{
	return 0;
}

BOOL SetupDiGetDeviceInstanceIdWHook(
	HDEVINFO         DeviceInfoSet,
	PSP_DEVINFO_DATA DeviceInfoData,
	PWSTR            DeviceInstanceId,
	DWORD            DeviceInstanceIdSize,
	PDWORD           RequiredSize
	)
{
	return 0;
}
