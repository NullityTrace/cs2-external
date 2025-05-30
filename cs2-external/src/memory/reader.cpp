#include <thread>
#include <map>
#include <cmath>
#include "../updater/offsets.h"
#include "reader.h"

void Reader::init() {
    HandleInspector inspector;
    if (inspector.FindHandlesToProcess(L"cs2.exe")) {
        HANDLE hCs2 = inspector.GetDuplicatedHandle();
        DWORD pid = inspector.GetDuplicatedPid();

        if (hCs2 && pid) {
            std::wcout << L"[cs2] Handle duplicado com sucesso! Handle: " << hCs2 << std::endl;

            process = std::make_shared<pProcess>(pid, hCs2);

            do {
                base_client = process->GetModule("client.dll");
                base_engine = process->GetModule("engine2.dll");

                if (base_client.base == 0 || base_engine.base == 0) {
                    std::cout << "[cs2] Aguardando client.dll/engine2.dll serem carregadas..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            } while (base_client.base == 0 || base_engine.base == 0);

            std::cout << "[cs2] base_engine.base: 0x" << std::hex << base_engine.base << std::endl;
            std::cout << "[cs2] dwBuildNumber offset: 0x" << std::hex << offsets::engine::dwBuildNumber << std::endl;

            uintptr_t address = base_engine.base + offsets::engine::dwBuildNumber;
            std::cout << "[cs2] Final read address: 0x" << std::hex << address << std::endl;

            uintptr_t buildNum = process->read<uintptr_t>(address);
            std::cout << "[cs2] BuildNumber: " << std::dec << buildNum << std::endl;
        }
    }
}



ProcessModule pProcess::GetModule(const char* lModule)
{
    std::wstring wideModule;
    int wideCharLength = MultiByteToWideChar(CP_UTF8, 0, lModule, -1, nullptr, 0);
    if (wideCharLength > 0)
    {
        wideModule.resize(wideCharLength);
        MultiByteToWideChar(CP_UTF8, 0, lModule, -1, &wideModule[0], wideCharLength);
    }

    HANDLE handle_module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid_);
    MODULEENTRY32W module_entry_{};
    module_entry_.dwSize = sizeof(MODULEENTRY32W);

    if (Module32FirstW(handle_module, &module_entry_)) {
        do {
            if (!wcscmp(module_entry_.szModule, wideModule.c_str())) {
                CloseHandle(handle_module);
                return { (uintptr_t)module_entry_.modBaseAddr, module_entry_.dwSize };
            }
        } while (Module32NextW(handle_module, &module_entry_));
    }

    CloseHandle(handle_module);
    return { 0, 0 };
}
