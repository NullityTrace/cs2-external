#include "pProcess.h"

bool pProcess::read_raw(uintptr_t address, void* buffer, size_t size) {
    SIZE_T bytesRead;
    if (!ReadProcessMemory(handle_, (LPCVOID)address, buffer, size, &bytesRead) || bytesRead != size) {
        std::cerr << "[!] ReadProcessMemory failed at 0x" << std::hex << address
            << " | Error: " << GetLastError() << std::endl;
        return false;
    }
    return true;
}
