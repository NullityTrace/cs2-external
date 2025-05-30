#pragma once

#include <Windows.h>
#include <winternl.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <ntstatus.h>
#define WIN32_NO_STATUS


#pragma comment(lib, "ntdll.lib")

typedef NTSTATUS(WINAPI* _NtQuerySystemInformation)(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
    );

#define SystemHandleInformation 16

typedef struct _SYSTEM_HANDLE {
    ULONG ProcessId;
    BYTE ObjectTypeNumber;
    BYTE Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, * PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
    ULONG HandleCount;
    SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

class HandleInspector {
public:
    void FindHandlesToNotepad();

private:
    DWORD GetNotepadPid();
    std::wstring GetProcessName(DWORD pid);
};
