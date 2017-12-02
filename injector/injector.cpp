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

// check the bits of OS and process
inline bool BitsCheck(HANDLE hProcess)
{
    HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32"));
    ISWOW64PROCESS pIsWow64Process;
    pIsWow64Process = (ISWOW64PROCESS)GetProcAddress(hKernel32, "IsWow64Process");

    BOOL bIsOSWin64 = FALSE, bIsWow64 = FALSE;
    if (pIsWow64Process)
    {
        bIsOSWin64 = TRUE;
        (*pIsWow64Process)(hProcess, &bIsWow64);
    }

#ifdef _WIN64
    return bIsOSWin64 && !bIsWow64;
#else
    return !bIsOSWin64 || bIsWow64;
#endif
}

// do DLL injection to execute LoadLibraryA with parameter in the process
bool DllInjectByPid(int pid, const char *dll_name)
{
    // get full path of payload DLL.
    char dll_path[MAX_PATH];
    GetModuleFileNameA(NULL, dll_path, MAX_PATH);
    char *pch = std::strrchr(dll_path, '\\');
    assert(pch);
    lstrcpyA(pch, "\\");
    lstrcatA(pch, dll_name);
    output("dll_path: %s\n", dll_path);

    // get process handle
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
    {
        output("pid is invalid\n");
        return false;
    }

    if (!BitsCheck(hProcess))
    {
        output("BitsCheck failed\n");
        CloseHandle(hProcess);
        return false;
    }

    // create memory block in remote process
    DWORD size = (lstrlenA(dll_path) + 1) * sizeof(char);
    LPVOID pParam = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT, PAGE_READWRITE);
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
        // execute LoadLibraryA with parameter
        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
            (LPTHREAD_START_ROUTINE)pLoadLibraryA, pParam, 0, NULL);
        if (hThread)
        {
            DWORD dwExitCode, dwError = GetLastError();
            WaitForSingleObject(hThread, INFINITE);
            GetExitCodeThread(hThread, &dwExitCode);
            CloseHandle(hThread);
            if (dwExitCode)
            {
                output("injected\n");
                ret = true;
            }
            else
            {
                output("LoadLibraryA failed: %ld\n", dwError);
            }
        }
        else
        {
            DWORD dwError = GetLastError();
            output("CreateRemoteThread failed: %ld\n", dwError);
        }
    }
    else
    {
        output("GetProcAddress failed\n");
    }

    VirtualFreeEx(hProcess, pParam, size, MEM_RELEASE);
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

bool DllInjectByExeName(const char *exe_name, const char *dll_name)
{
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

    PROCESSENTRY32 pe32;
    ZeroMemory(&pe32, sizeof(pe32));
    pe32.dwSize = sizeof(pe32);

    bool bFound = false, bSuccess = false;
    if (Process32First(hSnapshot, &pe32))
    {
        do
        {
            const char *pch = GetPathTitleA(pe32.szExeFile);
            if (lstrcmpiA(pch, exe_name) != 0)
                continue;

            bFound = true;

            DWORD pid = pe32.th32ProcessID;
            if (DllInjectByPid(pid, dll_name))
            {
                bSuccess = true;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);

    if (!bFound)
        output("process not found\n");
    else if (!bSuccess)
        output("DllInjectByPid failed\n");
    else
        output("DllInjectByExeName succeeded!\n");

    return bSuccess;
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
        int pid = atoi(argv[2]);
        ok = DllInjectByPid(pid, PAYLOAD_NAME ".dll");
    }
    else if (lstrcmpiA(argv[1], "-e") == 0 || lstrcmpiA(argv[1], "/e") == 0)
    {
        ok = DllInjectByExeName(argv[2], PAYLOAD_NAME ".dll");
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
