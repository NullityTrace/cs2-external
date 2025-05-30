#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

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
        SIZE_T bytesRead = 0;

        if (!ReadProcessMemory(handle_, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesRead)) {
            DWORD error = GetLastError();
            std::cerr << "[!] ReadProcessMemory failed at 0x" << std::hex << address
                << " | Error: " << std::dec << error << std::endl;
        }
        else if (bytesRead != sizeof(T)) {
            std::cerr << "[!] Incomplete read at 0x" << std::hex << address
                << " | Expected: " << sizeof(T) << ", Got: " << bytesRead << std::endl;
        }

        return buffer;
    }

    HWND findMainWindow();
};
