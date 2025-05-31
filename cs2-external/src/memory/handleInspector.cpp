#include "HandleInspector.h"
#include "pProcess.h"


DWORD HandleInspector::GetPidByName(const std::wstring& processName) {
    DWORD pid = 0;
    PROCESSENTRY32 entry = { sizeof(entry) };

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        if (Process32First(snapshot, &entry)) {
            do {
                if (_wcsicmp(entry.szExeFile, processName.c_str()) == 0) {
                    pid = entry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &entry));
        }
        CloseHandle(snapshot);
    }

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


HWND GetWindowHandleFromProcessId(DWORD ProcessId) {
    HWND hwnd = NULL;
    do {
        hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        if (pid == ProcessId) {
            TCHAR windowTitle[MAX_PATH];
            GetWindowText(hwnd, windowTitle, MAX_PATH);
            if (IsWindowVisible(hwnd) && windowTitle[0] != '\0') {
                return hwnd;
            }
        }
    } while (hwnd != NULL);
    return NULL; 
}

bool HandleInspector::FindHandlesToProcess(const std::wstring& targetProcessName) {
    DWORD targetPid = GetPidByName(targetProcessName);
    if (!targetPid) {
        std::wcout << L"[!] Process not found: " << targetProcessName << std::endl;
        return false;
    }

    _NtQuerySystemInformation NtQuerySystemInformation =
        (_NtQuerySystemInformation)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQuerySystemInformation");

    if (!NtQuerySystemInformation) {
        std::wcerr << L"[!] Failed to resolve NtQuerySystemInformation." << std::endl;
        return false;
    }

    ULONG handleInfoSize = 0x10000;
    PSYSTEM_HANDLE_INFORMATION handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);
    ULONG returnLength = 0;

    while (NtQuerySystemInformation(SystemHandleInformation, handleInfo, handleInfoSize, &returnLength) == STATUS_INFO_LENGTH_MISMATCH) {
        handleInfoSize *= 2;
        handleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize);
    }

    for (ULONG i = 0; i < handleInfo->HandleCount; i++) {
        SYSTEM_HANDLE h = handleInfo->Handles[i];

        if (h.ProcessId == GetCurrentProcessId()) continue;

        HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, h.ProcessId);
        if (!hProcess) continue;

        HANDLE dupHandle = nullptr;
        if (DuplicateHandle(hProcess, (HANDLE)(uintptr_t)h.Handle,
            GetCurrentProcess(), &dupHandle, PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, 0)
            ) {

            DWORD foundPid = GetProcessId(dupHandle);
            if (foundPid == targetPid) {
                std::wcout << L"[+] Found duplicated handle to " << targetProcessName
                    << L" (PID: " << foundPid << L") from process PID: " << h.ProcessId << std::endl;

                duplicatedHandle_ = dupHandle;
                duplicatedPid_ = foundPid;
                CloseHandle(hProcess);
                free(handleInfo);
                return true;
            }

            CloseHandle(dupHandle);
        }

        CloseHandle(hProcess);
    }

    free(handleInfo);
    return false;
}

HANDLE HandleInspector::GetDuplicatedHandle() const {
    return duplicatedHandle_;
}

void HandleInspector::CloseDuplicatedHandle() {
    if (duplicatedHandle_) {
        CloseHandle(duplicatedHandle_);
        duplicatedHandle_ = nullptr;
    }
}
