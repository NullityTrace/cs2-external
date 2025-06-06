#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <Psapi.h> 

typedef NTSTATUS(WINAPI* pNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesRead);
typedef NTSTATUS(WINAPI* pNtWriteVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);

class pMemory {
public:
    pMemory() {
        pfnNtReadVirtualMemory = (pNtReadVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtReadVirtualMemory");
    }

    pNtReadVirtualMemory pfnNtReadVirtualMemory;
};


struct ProcessModule {
    uintptr_t base, size;
};

class pProcess {
public:
    DWORD pid_ = 0;
    HANDLE handle_ = nullptr;
    HWND hwnd_ = nullptr;

    bool read_raw(uintptr_t address, void* buffer, size_t size);

    pProcess() = default;
    pProcess(DWORD pid, HANDLE handle) : pid_(pid), handle_(handle) {}

    ProcessModule GetModule(const char* module_name);

    template<class T>
    T read(uintptr_t address)
    {
        T buffer{};
        pMemory cMemory;

        cMemory.pfnNtReadVirtualMemory(handle_, (void*)address, &buffer, sizeof(T), 0);
        return buffer;
    }

    HWND findMainWindow();
};


