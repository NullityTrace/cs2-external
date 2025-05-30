#include "HandleInspector.h"



DWORD HandleInspector::GetNotepadPid() {
    DWORD pid = 0;
    PROCESSENTRY32 entry = {};
    entry.dwSize = sizeof(entry);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, L"notepad.exe") == 0) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return pid;
}

std::wstring HandleInspector::GetProcessName(DWORD pid) {
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProc) return L"<Access Denied>";

    wchar_t name[MAX_PATH] = L"<Unknown>";
    if (GetModuleBaseNameW(hProc, nullptr, name, MAX_PATH)) {
        CloseHandle(hProc);
        return std::wstring(name);
    }
    CloseHandle(hProc);
    return L"<Unknown>";
}

void HandleInspector::FindHandlesToNotepad() {
    DWORD notepadPid = GetNotepadPid();
    if (!notepadPid) {
        std::wcout << L"Notepad.exe not found." << std::endl;
        return;
    }

    HANDLE hNotepad = OpenProcess(PROCESS_ALL_ACCESS, FALSE, notepadPid);
    if (!hNotepad) {
        std::wcout << L"Failed to open notepad process." << std::endl;
        return;
    }

    _NtQuerySystemInformation NtQuerySystemInformation = (_NtQuerySystemInformation)GetProcAddress(
        GetModuleHandleW(L"ntdll.dll"), "NtQuerySystemInformation"
    );

    if (!NtQuerySystemInformation) {
        std::wcerr << L"Failed to resolve NtQuerySystemInformation." << std::endl;
        return;
    }

    ULONG handleInfoSize = 0x10000;
    PSYSTEM_HANDLE_INFORMATION handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);
    ULONG returnLength = 0;

    while (NtQuerySystemInformation(SystemHandleInformation, handleInfo, handleInfoSize, &returnLength) == STATUS_INFO_LENGTH_MISMATCH) {
        handleInfoSize *= 2;
        handleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize);
    }

    std::map<DWORD, bool> reported;

    for (ULONG i = 0; i < handleInfo->HandleCount; i++) {
        SYSTEM_HANDLE h = handleInfo->Handles[i];

        if (h.ProcessId == GetCurrentProcessId()) continue;

        HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, h.ProcessId);
        if (!hProcess) continue;

        HANDLE dupHandle = nullptr;
        if (DuplicateHandle(hProcess, (HANDLE)(uintptr_t)h.Handle,
            GetCurrentProcess(), &dupHandle, 0, FALSE, DUPLICATE_SAME_ACCESS)) {

            DWORD targetPid = GetProcessId(dupHandle);
            if (targetPid == notepadPid && !reported[h.ProcessId]) {
                std::wcout << L"Process " << GetProcessName(h.ProcessId)
                    << L" (PID: " << h.ProcessId << L") has a handle to notepad.exe" << std::endl;
                std::wcout << L"Duplicated handle: " << dupHandle
                    << L" | Target PID: " << targetPid << std::endl;
                reported[h.ProcessId] = true;
            }
            CloseHandle(dupHandle);
        }

        CloseHandle(hProcess);
    }

    free(handleInfo);
    CloseHandle(hNotepad);
}
