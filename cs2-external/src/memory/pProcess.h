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

    template <typename T>
    T read(uintptr_t address) {
        T buffer{};
        if (!read_raw(address, &buffer, sizeof(T))) {
            DWORD error = GetLastError();
            std::cerr << "[!] read_raw failed at 0x" << std::hex << address
                << " | Error: " << std::dec << error << std::endl;
        }
        return buffer;
    }

    HWND findMainWindow();
};

class pMemory {

public:
    pMemory() {
        pfnNtReadVirtualMemory = (pNtReadVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtReadVirtualMemory");
    }

    pNtReadVirtualMemory pfnNtReadVirtualMemory;
};
