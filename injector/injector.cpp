// injector by katahiromz
//////////////////////////////////////////////////////////////////////////////

#include "config.hpp"
#include "targetver.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <mbstring.h>

typedef BOOL (WINAPI *ISWOW64PROCESS)(HANDLE, PBOOL);

// output to console and debugger
void output(const char *psz, ...)
{
    char buf[512];
    va_list va;
    va_start(va, psz);

    wvsprintfA(buf, psz, va);

    OutputDebugStringA(buf);
    fputs(buf, stdout);

    va_end(va);
}

// check the bits of processor and process
inline bool CheckBits(HANDLE hProcess)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    output("wProcessorArchitecture: %u\n", info.wProcessorArchitecture);

    #ifdef _WIN64
        return (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64);
    #else
        return (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL);
    #endif
}

bool GetProcessThreads(DWORD pid, std::vector<DWORD>& tids)
{
    HANDLE hSnapshot;
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        output("CreateToolhelp32Snapshot failed");
        return false;
    }

    #undef THREADENTRY32
    #undef Thread32First
    #undef Thread32Next
    THREADENTRY32 te32 = { sizeof(te32) };

    if (Thread32First(hSnapshot, &te32))
    {
        do
        {
            if (te32.th32OwnerProcessID == pid)
            {
                tids.push_back(te32.th32ThreadID);
            }
        } while (Thread32Next(hSnapshot, &te32));
    }

    return !tids.empty();
}

// do DLL injection to execute LoadLibraryA with parameter in the process
bool DllInjectByPid(DWORD pid, const char *dll_name)
{
    std::vector<DWORD> tids;
    if (!GetProcessThreads(pid, tids))
    {
        output("GetProcessThreads failed\n");
        return false;
    }

    // get full path of payload DLL.
    char dll_path[MAX_PATH];
    GetModuleFileNameA(NULL, dll_path, MAX_PATH);
    char *pch = std::strrchr(dll_path, '\\');
    assert(pch);
    lstrcpyA(pch, "\\");
    lstrcatA(pch, dll_name);
    output("dll_path: %s\n", dll_path);

    // get process handle
    HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (!hProcess)
    {
        output("pid is invalid\n");
        return false;
    }

    if (!CheckBits(hProcess))
    {
        output("CheckBits failed\n");
        CloseHandle(hProcess);
        return false;
    }

    // create memory block in remote process
    DWORD size = (lstrlenA(dll_path) + 1) * sizeof(char);
    LPVOID pParam = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pParam)
    {
        output("VirtualAllocEx failed\n");
        CloseHandle(hProcess);
        return false;
    }

    // write path
    WriteProcessMemory(hProcess, pParam, dll_path, size, NULL);

    // get procedure address of LoadLibraryA
    bool ret = false;
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    FARPROC pLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");
    if (pLoadLibraryA)
    {
        for (size_t i = 0; i < tids.size(); ++i)
        {
            HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, tids[i]);
            if (hThread)
            {
                if (QueueUserAPC((PAPCFUNC)pLoadLibraryA, hThread, pParam))
                    ret = true;

                CloseHandle(hThread);
            }
        }
    }
    else
    {
        output("GetProcAddress failed\n");
    }

    VirtualFreeEx(hProcess, pParam, size, MEM_RELEASE | MEM_DECOMMIT);
    CloseHandle(hProcess);

    return ret;
}

const char *GetPathTitleA(const char *path)
{
    const char *pch;
    const char *pch1 = (const char *)_mbsrchr((const BYTE *)path, '/');
    const char *pch2 = (const char *)_mbsrchr((const BYTE *)path, '\\');

    if (pch1 && pch2)
    {
        pch = ((pch1 < pch2) ? pch2 : pch1) + 1;
    }
    else if (pch1)
    {
        pch = pch1 + 1;
    }
    else if (pch2)
    {
        pch = pch2 + 1;
    }
    else
    {
        pch = path;
    }

    return pch;
}

bool FindProcess(const char *exe_name, DWORD& pid)
{
    pid = 0;

    HANDLE hSnapshot;
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        output("CreateToolhelp32Snapshot failed");
        return false;
    }

    // use ANSI version
    #undef PROCESSENTRY32
    #undef Process32First
    #undef Process32Next

    PROCESSENTRY32 pe32 = { sizeof(pe32) };

    if (Process32First(hSnapshot, &pe32))
    {
        do
        {
            const char *pch = GetPathTitleA(pe32.szExeFile);
            if (lstrcmpiA(pch, exe_name) != 0)
                continue;

            pid = pe32.th32ProcessID;
            break;
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);

    return pid != 0;
}

BOOL EnableProcessPriviledge(LPCTSTR pszSE_)
{
    BOOL f;
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tp;
    
    f = FALSE;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        if (LookupPrivilegeValue(NULL, pszSE_, &luid))
        {
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            tp.Privileges[0].Luid = luid;
            f = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);
        }
        CloseHandle(hToken);
    }
    
    return f;
}

void show_help(void)
{
    output("Usage #1: %s -p pid\n", INJECTOR_NAME);
    output("Usage #2: %s -e exe_file.exe\n", INJECTOR_NAME);
    output("\n");
    output("pid is a process ID\n");
}

// the main function
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        show_help();
        return 1;
    }

    if (!EnableProcessPriviledge(SE_DEBUG_NAME))
    {
        output("Unable to enable priviledge\n");
    }

    bool ok = false;
    if (lstrcmpiA(argv[1], "-p") == 0 || lstrcmpiA(argv[1], "/p") == 0)
    {
        DWORD pid = atoi(argv[2]);
        ok = DllInjectByPid(pid, PAYLOAD_NAME ".dll");
    }
    else if (lstrcmpiA(argv[1], "-e") == 0 || lstrcmpiA(argv[1], "/e") == 0)
    {
        DWORD pid;
        if (FindProcess(argv[2], pid))
        {
            ok = DllInjectByPid(pid, PAYLOAD_NAME ".dll");
        }
    }
    else
    {
        show_help();
        return 2;
    }

    if (ok)
    {
        puts("Gotcha!");
        return 0;
    }

    output("failed\n");
    return 3;
}
