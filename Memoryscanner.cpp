#include "MemoryScanner.h"

MemoryScanner::MemoryScanner() : hProcess(nullptr), processID(0) {}

MemoryScanner::~MemoryScanner() {
    if (hProcess) {
        CloseHandle(hProcess);
    }
}

bool MemoryScanner::Attach(const std::wstring& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            if (processName == pe32.szExeFile) {
                processID = pe32.th32ProcessID;
                hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, processID);
                CloseHandle(hSnapshot);
                return hProcess != nullptr;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    return false;
}

uintptr_t MemoryScanner::GetModuleBase(const std::wstring& moduleName) const {
    if (!processID) return 0;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    MODULEENTRY32W me32;
    me32.dwSize = sizeof(MODULEENTRY32W);
    uintptr_t moduleBase = 0;

    if (Module32FirstW(hSnapshot, &me32)) {
        do {
            if (moduleName == me32.szModule) {
                moduleBase = (uintptr_t)me32.modBaseAddr;
                break;
            }
        } while (Module32NextW(hSnapshot, &me32));
    }
    CloseHandle(hSnapshot);
    return moduleBase;
}

bool MemoryScanner::ReadMemoryBlock(uintptr_t address, std::vector<uint8_t>& buffer, size_t size) const {
    buffer.resize(size);
    SIZE_T bytesRead;
    return ReadProcessMemory(hProcess, (LPCVOID)address, buffer.data(), size, &bytesRead) && bytesRead == size;
}