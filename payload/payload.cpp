// SimpleDLLInjection by katahiromz
//////////////////////////////////////////////////////////////////////////////

#include "config.hpp"
#include "targetver.h"
#include <windows.h>
#include <imagehlp.h>
#include <tchar.h>
#include <cstdio>
#include <cstring>

#ifndef _countof
    #define _countof(array)     (sizeof(array) / sizeof(array[0]))
#endif

HINSTANCE g_hinstDLL;
HINSTANCE g_hTargetEXE;

//////////////////////////////////////////////////////////////////////////////

// output to console and debugger
void output(const char *fmt, ...)
{
    DWORD dwError = GetLastError();

    char buf[512];
    va_list va;
    va_start(va, fmt);
    wvsprintfA(buf, fmt, va);
    va_end(va);

    TCHAR szPath[MAX_PATH];
    GetModuleFileName(g_hinstDLL, szPath, MAX_PATH);
    TCHAR *pch = _tcsrchr(szPath, TEXT('\\'));
    if (pch == NULL)
        pch = _tcsrchr(szPath, TEXT('/'));
    lstrcpy(pch + 1, TEXT("hookapi.txt"));

    HANDLE hMutex = CreateMutex(NULL, FALSE, TEXT("SysNotifyHooker Mutex"));
    WaitForSingleObject(hMutex, 800);
    {
        using namespace std;
        FILE *fp = _tfopen(szPath, TEXT("a"));
        if (fp)
        {
            fputs(buf, fp);
            fflush(fp);
            fclose(fp);
        }
    }
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);

    SetLastError(dwError);
}

//////////////////////////////////////////////////////////////////////////////

// user32: MessageBox
typedef INT (WINAPI *FN_MESSAGEBOXA)(HWND, const char *, const char *, UINT);
typedef INT (WINAPI *FN_MESSAGEBOXW)(HWND, const WCHAR *, const WCHAR *, UINT);
// user32: PostMessage
typedef BOOL (WINAPI *FN_POSTMESSAGEA)(HWND, UINT, WPARAM, LPARAM);
typedef BOOL (WINAPI *FN_POSTMESSAGEW)(HWND, UINT, WPARAM, LPARAM);
// user32: SendMessage
typedef LRESULT (WINAPI *FN_SENDMESSAGEA)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT (WINAPI *FN_SENDMESSAGEW)(HWND, UINT, WPARAM, LPARAM);
// user32: SendNotifyMessage
typedef BOOL (WINAPI *FN_SENDNOTIFYMESSAGEA)(HWND, UINT, WPARAM, LPARAM);
typedef BOOL (WINAPI *FN_SENDNOTIFYMESSAGEW)(HWND, UINT, WPARAM, LPARAM);
// user32: BroadcastSystemMessage
typedef LONG (WINAPI *FN_BROADCASTSYSTEMMESSAGEA)(DWORD, LPDWORD, UINT, WPARAM, LPARAM);
typedef LONG (WINAPI *FN_BROADCASTSYSTEMMESSAGEW)(DWORD, LPDWORD, UINT, WPARAM, LPARAM);
// shell32: SHChangeNotify
typedef void (STDAPICALLTYPE *FN_SHCHANGENOTIFY)(LONG, UINT, LPCVOID, LPCVOID);

FN_MESSAGEBOXA pMessageBoxA = NULL;
FN_MESSAGEBOXW pMessageBoxW = NULL;

FN_POSTMESSAGEA pPostMessageA = NULL;
FN_POSTMESSAGEW pPostMessageW = NULL;

FN_SENDMESSAGEA pSendMessageA = NULL;
FN_SENDMESSAGEW pSendMessageW = NULL;

FN_SENDNOTIFYMESSAGEA pSendNotifyMessageA = NULL;
FN_SENDNOTIFYMESSAGEW pSendNotifyMessageW = NULL;

FN_BROADCASTSYSTEMMESSAGEA pBroadcastSystemMessageA = NULL;
FN_BROADCASTSYSTEMMESSAGEW pBroadcastSystemMessageW = NULL;

FN_SHCHANGENOTIFY pSHChangeNotify = NULL;

//////////////////////////////////////////////////////////////////////////////
// user32

extern "C" {

__declspec(dllexport)
INT WINAPI NewMessageBoxA(HWND hwnd, const char *text, const char *title, UINT uType)
{
    INT ret = 0;
    if (pMessageBoxA)
    {
        output("pMessageBoxA: enter: (%p, %s, %s, %u);\n", hwnd, text, title, uType);
        ret = (*pMessageBoxA)(hwnd, "OK, Hooked", title, uType);
        output("pMessageBoxA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
INT WINAPI NewMessageBoxW(HWND hwnd, const WCHAR *text, const WCHAR *title, UINT uType)
{
    INT ret = 0;
    if (pMessageBoxW)
    {
        output("pMessageBoxW: enter: (%p, %ls, %ls, %u);\n", hwnd, text, title, uType);
        ret = (*pMessageBoxW)(hwnd, L"OK, Hooked", title, uType);
        output("pMessageBoxW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI NewPostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret = 0;
    if (pPostMessageA)
    {
        output("PostMessageA: enter: (%p, %u, %p, %p);\n", hWnd, Msg, wParam, lParam);
        ret = (*pPostMessageA)(hWnd, Msg, wParam, lParam);
        output("PostMessageA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI NewPostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret = FALSE;
    if (pPostMessageW)
    {
        output("PostMessageW: enter: (%p, %u, %p, %p);\n", hWnd, Msg, wParam, lParam);
        ret = (*pPostMessageW)(hWnd, Msg, wParam, lParam);
        output("PostMessageW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
LRESULT WINAPI NewSendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 0;
    if (pSendMessageA)
    {
        output("SendMessageA: enter: (%p, %u, %p, %p);\n", hWnd, Msg, wParam, lParam);
        ret = (*pSendMessageA)(hWnd, Msg, wParam, lParam);
        output("SendMessageA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
LRESULT WINAPI NewSendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = FALSE;
    if (pSendMessageW)
    {
        output("SendMessageW: enter: (%p, %u, %p, %p);\n", hWnd, Msg, wParam, lParam);
        ret = (*pSendMessageW)(hWnd, Msg, wParam, lParam);
        output("SendMessageW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI NewSendNotifyMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret = FALSE;
    if (pSendNotifyMessageA)
    {
        output("SendNotifyMessageA: enter: (%p, %u, %p, %p);\n", hWnd, Msg, wParam, lParam);
        ret = (*pSendNotifyMessageA)(hWnd, Msg, wParam, lParam);
        output("SendNotifyMessageA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI NewSendNotifyMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret = FALSE;
    if (pSendNotifyMessageW)
    {
        output("SendNotifyMessageW: enter: (%p, %u, %p, %p);\n", hWnd, Msg, wParam, lParam);
        ret = (*pSendNotifyMessageW)(hWnd, Msg, wParam, lParam);
        output("SendNotifyMessageW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
INT WINAPI NewBroadcastSystemMessageA(DWORD dwFlags, LPDWORD lpdwRecipients, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
    INT nRet = 0;
    if (pBroadcastSystemMessageA)
    {
        output("BroadcastSystemMessageA: enter: (0x%08lX, %p, %u, %p, %p);\n", dwFlags, lpdwRecipients, uiMessage, wParam, lParam);
        nRet = (*pBroadcastSystemMessageA)(dwFlags, lpdwRecipients, uiMessage, wParam, lParam);
        output("BroadcastSystemMessageA: leave: nRet = %d;\n", nRet);
    }
    return nRet;
}

__declspec(dllexport)
INT WINAPI NewBroadcastSystemMessageW(DWORD dwFlags, LPDWORD lpdwRecipients, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
    INT nRet = 0;
    if (pBroadcastSystemMessageW)
    {
        output("BroadcastSystemMessageW: enter: (0x%08lX, %p, %u, %p, %p);\n", dwFlags, lpdwRecipients, uiMessage, wParam, lParam);
        nRet = (*pBroadcastSystemMessageW)(dwFlags, lpdwRecipients, uiMessage, wParam, lParam);
        output("BroadcastSystemMessageW: leave: nRet = %d;\n", nRet);
    }
    return nRet;
}

} // extern "C"

//////////////////////////////////////////////////////////////////////////////
// shell32

extern "C" {

__declspec(dllexport)
void STDAPICALLTYPE NewSHChangeNotify(LONG wEventId, UINT uFlags, LPCVOID dwItem1, LPCVOID dwItem2)
{
    if (pSHChangeNotify)
    {
        output("SHChangeNotify: enter: (%ld, 0x%08lX, %p, %p);\n", wEventId, uFlags, dwItem1, dwItem2);
        (*pSHChangeNotify)(wEventId, uFlags, dwItem1, dwItem2);
        output("SHChangeNotify: leave;\n");
    }
}

} // extern "C"

//////////////////////////////////////////////////////////////////////////////

FARPROC
ApiHookModule(HMODULE hMod, const char *dll_name, const char *fn_name, FARPROC fnNew)
{
    if (hMod == NULL)
        hMod = GetModuleHandle(NULL);

    BYTE *pbMod = (BYTE *)hMod;
    DWORD dwDataSize;
    IMAGE_IMPORT_DESCRIPTOR *pImpDesc = (IMAGE_IMPORT_DESCRIPTOR *)
        ImageDirectoryEntryToData(hMod, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &dwDataSize);

    for (; pImpDesc->Name; ++pImpDesc)
    {
        char *entry_dll_name = (char *)(pbMod + pImpDesc->Name);
        output("entry_dll_name: %s, dll_name: %s\n", entry_dll_name, dll_name);
        if (!stricmp(entry_dll_name, dll_name))
            break;
    }
    if (!pImpDesc->Name)
    {
        output("ApiHookModule: !pImpDesc->Name\n");
        return NULL;
    }

    BOOL bOrdinal = (HIWORD((DWORD_PTR)fn_name) == 0);

    IMAGE_THUNK_DATA *pINT = (IMAGE_THUNK_DATA *)(pbMod + pImpDesc->OriginalFirstThunk);
    IMAGE_THUNK_DATA *pIAT = (IMAGE_THUNK_DATA *)(pbMod + pImpDesc->FirstThunk);
    for (; pIAT->u1.Function; pIAT++, pINT++)
    {
        if (bOrdinal)
        {
            if (!IMAGE_SNAP_BY_ORDINAL(pINT->u1.Ordinal))
                continue;

            WORD wOrdinal = LOWORD((DWORD_PTR)fn_name);
            if (wOrdinal != IMAGE_ORDINAL(pINT->u1.Ordinal))
                continue;
        }
        else
        {
            if (IMAGE_SNAP_BY_ORDINAL(pINT->u1.Ordinal))
                continue;

            IMAGE_IMPORT_BY_NAME *pName =
                (IMAGE_IMPORT_BY_NAME *)(pbMod + pINT->u1.AddressOfData);

            if (stricmp((char *)pName->Name, fn_name) != 0)
                continue;
        }

        DWORD dwOldProtect, dwSize = sizeof(pIAT->u1.Function);
        if (VirtualProtect(&pIAT->u1.Function, dwSize, PAGE_READWRITE, &dwOldProtect))
        {
            FARPROC fnOld = (FARPROC)pIAT->u1.Function;
            pIAT->u1.Function = (DWORD_PTR)fnNew;
            VirtualProtect(&pIAT->u1.Function, dwSize, dwOldProtect, &dwOldProtect);
            output("ApiHookModule: success: %s\n", fn_name);
            return fnOld;
        }
    }

    output("ApiHookModule: %s: not found\n", fn_name);
    return NULL;
}

static BOOL hook(void)
{
    FARPROC fnOld;

    fnOld = ApiHookModule(NULL, "user32.dll", "MessageBoxA", (FARPROC)NewMessageBoxA);
    pMessageBoxA = (FN_MESSAGEBOXA)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "user32.dll", "MessageBoxW", (FARPROC)NewMessageBoxW);
    pMessageBoxW = (FN_MESSAGEBOXW)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "user32.dll", "PostMessageA", (FARPROC)NewPostMessageA);
    pPostMessageA = (FN_POSTMESSAGEA)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "user32.dll", "PostMessageW", (FARPROC)NewPostMessageW);
    pPostMessageW = (FN_POSTMESSAGEW)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "user32.dll", "SendMessageA", (FARPROC)NewSendMessageA);
    pSendMessageA = (FN_SENDMESSAGEA)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "user32.dll", "SendMessageW", (FARPROC)NewSendMessageW);
    pSendMessageW = (FN_SENDMESSAGEW)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "user32.dll", "SendNotifyMessageA", (FARPROC)NewSendNotifyMessageA);
    pSendNotifyMessageA = (FN_SENDNOTIFYMESSAGEA)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "user32.dll", "SendNotifyMessageW", (FARPROC)NewSendNotifyMessageW);
    pSendNotifyMessageW = (FN_SENDNOTIFYMESSAGEW)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "user32.dll", "BroadcastSystemMessageA", (FARPROC)NewBroadcastSystemMessageA);
    pBroadcastSystemMessageA = (FN_BROADCASTSYSTEMMESSAGEA)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "user32.dll", "BroadcastSystemMessageW", (FARPROC)NewBroadcastSystemMessageW);
    pBroadcastSystemMessageW = (FN_BROADCASTSYSTEMMESSAGEW)fnOld;
    if (!fnOld)
        return FALSE;

    fnOld = ApiHookModule(NULL, "shell32.dll", "SHChangeNotify", (FARPROC)NewSHChangeNotify);
    pSHChangeNotify = (FN_SHCHANGENOTIFY)fnOld;
    if (!fnOld)
        return FALSE;

    return TRUE;
}

static void unhook(void)
{
    ApiHookModule(NULL, "user32.dll", "PostMessageA", (FARPROC)pPostMessageA);
    pPostMessageA = NULL;

    ApiHookModule(NULL, "user32.dll", "PostMessageW", (FARPROC)pPostMessageW);
    pPostMessageW = NULL;

    ApiHookModule(NULL, "user32.dll", "SendMessageA", (FARPROC)pSendMessageA);
    pSendMessageA = NULL;

    ApiHookModule(NULL, "user32.dll", "SendMessageW", (FARPROC)pSendMessageW);
    pSendMessageW = NULL;

    ApiHookModule(NULL, "user32.dll", "SendNotifyMessageA", (FARPROC)pSendNotifyMessageA);
    pSendNotifyMessageA = NULL;

    ApiHookModule(NULL, "user32.dll", "SendNotifyMessageW", (FARPROC)pSendNotifyMessageW);
    pSendNotifyMessageW = NULL;

    ApiHookModule(NULL, "user32.dll", "BroadcastSystemMessageA", (FARPROC)pBroadcastSystemMessageA);
    pBroadcastSystemMessageA = NULL;

    ApiHookModule(NULL, "user32.dll", "BroadcastSystemMessageW", (FARPROC)pBroadcastSystemMessageW);
    pBroadcastSystemMessageW = NULL;

    ApiHookModule(NULL, "shell32.dll", "SHChangeNotify", (FARPROC)pSHChangeNotify);
    pSHChangeNotify = NULL;
}

extern "C"
BOOL WINAPI
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        g_hinstDLL = hinstDLL;
        output("DLL_PROCESS_ATTACH\n");
        hook();
        break;
    case DLL_PROCESS_DETACH:
        output("DLL_PROCESS_DETACH\n");
        unhook();
        break;
    case DLL_THREAD_ATTACH:
        output("DLL_THREAD_ATTACH\n");
        break;
    case DLL_THREAD_DETACH:
        output("DLL_THREAD_DETACH\n");
        break;
    }

    return TRUE;
}
