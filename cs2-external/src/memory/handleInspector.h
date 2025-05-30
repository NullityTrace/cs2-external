#pragma once

#include <Windows.h>
#include <winternl.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <ntstatus.h>

#pragma comment(lib, "ntdll.lib")

#define WIN32_NO_STATUS
#define SystemHandleInformation 16

typedef NTSTATUS(WINAPI* _NtQuerySystemInformation)(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
    );

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
    bool FindHandlesToProcess(const std::wstring& targetProcessName);
    HANDLE GetDuplicatedHandle() const;
    void CloseDuplicatedHandle();
    std::wstring GetProcessName(DWORD pid);
    DWORD GetPidByName(const std::wstring& processName);
    DWORD GetDuplicatedPid() const { return duplicatedPid_; } 

private:
    HANDLE duplicatedHandle_ = nullptr;
    DWORD duplicatedPid_ = 0; 
};




