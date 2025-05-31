#include "pProcess.h"

bool pProcess::read_raw(uintptr_t address, void* buffer, size_t size) {
    SIZE_T bytesRead = 0;
    static pMemory cMemory;

    if (!cMemory.pfnNtReadVirtualMemory) {
        std::cerr << "[!] pfnNtReadVirtualMemory é nulo!" << std::endl;
        return false;
    }

    NTSTATUS status = cMemory.pfnNtReadVirtualMemory(
        this->handle_,
        (PVOID)address,
        buffer,
        static_cast<ULONG>(size),
        (PULONG)&bytesRead
    );

    if (status != 0) {
        std::cerr << "[!] NtReadVirtualMemory falhou. Status: 0x"
            << std::hex << status << " | Endereço: 0x" << address << std::endl;
        return false;
    }

    return bytesRead == size;
}

