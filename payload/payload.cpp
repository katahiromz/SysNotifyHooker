// SysNotifyHooker payload by katahiromz
//////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include "config.hpp"
#include "targetver.h"
#include <windows.h>
#include <imagehlp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <tchar.h>
#include <cstdio>
#include <cstring>
#include <string>
#include "msgdump.hpp"

#ifndef _countof
    #define _countof(array)     (sizeof(array) / sizeof(array[0]))
#endif

HINSTANCE g_hinstDLL;

//////////////////////////////////////////////////////////////////////////////
// window handles

HWND Shell_TrayWnd = NULL;
HWND StartButton = NULL;
HWND TrayNotifyWnd = NULL;
HWND TrayClockWClass = NULL;
HWND Progman = NULL;

#define MAX_EXPLORER 16
INT ExplorerWndCount = 0;
HWND ExplorerWnd[MAX_EXPLORER] = { NULL };

std::string make_hwnd_text(HWND hwnd)
{
#define CHECK_HWND(hwnd, var) if (hwnd == var) return #var;
    CHECK_HWND(hwnd, Shell_TrayWnd);
    CHECK_HWND(hwnd, StartButton);
    CHECK_HWND(hwnd, TrayNotifyWnd);
    CHECK_HWND(hwnd, TrayClockWClass);
    CHECK_HWND(hwnd, Progman);
    for (INT i = 0; i < ExplorerWndCount; ++i)
    {
        CHECK_HWND(hwnd, ExplorerWnd[i]);
    }

    char buf[32];
    wsprintfA(buf, "%p", hwnd);
    return buf;
#undef CHECK_HWND
}

#define HWND2TEXT(hwnd) make_hwnd_text(hwnd).c_str()

//////////////////////////////////////////////////////////////////////////////

void log_printf(const char *fmt, ...)
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
    lstrcpy(pch + 1, TEXT("SysNotifyLog.txt"));

    HANDLE hMutex = CreateMutex(NULL, FALSE, TEXT("SysNotifyHooker Mutex"));
    WaitForSingleObject(hMutex, 800);
    {
        using namespace std;
        FILE *fp = _tfopen(szPath, TEXT("a"));
        if (fp)
        {
            fprintf(fp, "PID:%08lX:TID:%08lX:TIC:%08lX> ",
                    GetCurrentProcessId(), GetCurrentThreadId(), GetTickCount());
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
// Task #1: Add function types

// user32: MessageBox
typedef INT (WINAPI *FN_MessageBoxA)(HWND, const char *, const char *, UINT);
typedef INT (WINAPI *FN_MessageBoxW)(HWND, const WCHAR *, const WCHAR *, UINT);
// user32: PostMessage
typedef BOOL (WINAPI *FN_PostMessageA)(HWND, UINT, WPARAM, LPARAM);
typedef BOOL (WINAPI *FN_PostMessageW)(HWND, UINT, WPARAM, LPARAM);
// user32: SendMessage
typedef LRESULT (WINAPI *FN_SendMessageA)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT (WINAPI *FN_SendMessageW)(HWND, UINT, WPARAM, LPARAM);
// user32: SendNotifyMessage
typedef BOOL (WINAPI *FN_SendNotifyMessageA)(HWND, UINT, WPARAM, LPARAM);
typedef BOOL (WINAPI *FN_SendNotifyMessageW)(HWND, UINT, WPARAM, LPARAM);
// user32: SendMessageCallback
typedef BOOL (WINAPI *FN_SendMessageCallbackA)(HWND, UINT, WPARAM, LPARAM, SENDASYNCPROC, ULONG_PTR);
typedef BOOL (WINAPI *FN_SendMessageCallbackW)(HWND, UINT, WPARAM, LPARAM, SENDASYNCPROC, ULONG_PTR);
// user32: SendMessageTimeout
typedef LRESULT (WINAPI *FN_SendMessageTimeoutA)(HWND, UINT, WPARAM, LPARAM, UINT, UINT, LPDWORD);
typedef LRESULT (WINAPI *FN_SendMessageTimeoutW)(HWND, UINT, WPARAM, LPARAM, UINT, UINT, LPDWORD);
// user32: BroadcastSystemMessage
typedef LONG (WINAPI *FN_BroadcastSystemMessageA)(DWORD, LPDWORD, UINT, WPARAM, LPARAM);
typedef LONG (WINAPI *FN_BroadcastSystemMessageW)(DWORD, LPDWORD, UINT, WPARAM, LPARAM);
// user32: NotifyWinEvent
typedef void (WINAPI *FN_NotifyWinEvent)(DWORD, HWND, LONG, LONG);
// user32: RegisterDeviceNotificationW
// user32: UnregisterDeviceNotification
typedef HANDLE (WINAPI *FN_RegisterDeviceNotificationW)(HANDLE, LPVOID, DWORD);
typedef BOOL (WINAPI *FN_UnregisterDeviceNotification)(HANDLE);
// user32: SendDlgItemMessage
typedef LRESULT (WINAPI *FN_SendDlgItemMessageA)(HWND, int, UINT, WPARAM, LPARAM);
typedef LRESULT (WINAPI *FN_SendDlgItemMessageW)(HWND, int, UINT, WPARAM, LPARAM);

// shell32: SHChangeNotify
typedef void (STDAPICALLTYPE *FN_SHChangeNotify)(LONG, UINT, LPCVOID, LPCVOID);
// shell32: SHChangeNotification_Lock
// shell32: SHChangeNotification_Unlock
typedef HANDLE (STDAPICALLTYPE *FN_SHChangeNotification_Lock)(HANDLE, DWORD, LPITEMIDLIST **, LONG *);
typedef BOOL (STDAPICALLTYPE *FN_SHChangeNotification_Unlock)(HANDLE);
// shell32: SHChangeNotifyRegister
// shell32: SHChangeNotifyDeregister
typedef ULONG (STDAPICALLTYPE *FN_SHChangeNotifyRegister)(HWND, int, LONG, UINT, int, void *);
typedef BOOL (STDAPICALLTYPE *FN_SHChangeNotifyDeregister)(ULONG);
// shell32: SHChangeNotifySuspendResume
typedef BOOL (STDAPICALLTYPE *FN_SHChangeNotifySuspendResume)(BOOL, LPITEMIDLIST, BOOL, LONG);
// shell32: Shell_NotifyIcon
typedef BOOL (STDAPICALLTYPE *FN_Shell_NotifyIconA)(DWORD, PNOTIFYICONDATAA);
typedef BOOL (STDAPICALLTYPE *FN_Shell_NotifyIconW)(DWORD, PNOTIFYICONDATAW);

// advapi32: RegNotifyChangeKeyValue
typedef LONG (WINAPI *FN_RegNotifyChangeKeyValue)(HKEY, BOOL, DWORD, HANDLE, BOOL);

//////////////////////////////////////////////////////////////////////////////
// Task #2: Add function variables

FN_MessageBoxA pMessageBoxA = NULL;
FN_MessageBoxW pMessageBoxW = NULL;
FN_PostMessageA pPostMessageA = NULL;
FN_PostMessageW pPostMessageW = NULL;
FN_SendMessageA pSendMessageA = NULL;
FN_SendMessageW pSendMessageW = NULL;
FN_SendNotifyMessageA pSendNotifyMessageA = NULL;
FN_SendNotifyMessageW pSendNotifyMessageW = NULL;
FN_SendMessageCallbackA pSendMessageCallbackA = NULL;
FN_SendMessageCallbackW pSendMessageCallbackW = NULL;
FN_SendMessageTimeoutA pSendMessageTimeoutA = NULL;
FN_SendMessageTimeoutW pSendMessageTimeoutW = NULL;
FN_BroadcastSystemMessageA pBroadcastSystemMessageA = NULL;
FN_BroadcastSystemMessageW pBroadcastSystemMessageW = NULL;
FN_NotifyWinEvent pNotifyWinEvent = NULL;
FN_RegisterDeviceNotificationW pRegisterDeviceNotificationW = NULL;
FN_UnregisterDeviceNotification pUnregisterDeviceNotification = NULL;
FN_SendDlgItemMessageA pSendDlgItemMessageA = NULL;
FN_SendDlgItemMessageW pSendDlgItemMessageW = NULL;

FN_SHChangeNotify pSHChangeNotify = NULL;
FN_SHChangeNotification_Lock pSHChangeNotification_Lock = NULL;
FN_SHChangeNotification_Unlock pSHChangeNotification_Unlock = NULL;
FN_SHChangeNotifyRegister pSHChangeNotifyRegister = NULL;
FN_SHChangeNotifyDeregister pSHChangeNotifyDeregister = NULL;
FN_SHChangeNotifySuspendResume pSHChangeNotifySuspendResume = NULL;

FN_Shell_NotifyIconA pShell_NotifyIconA = NULL;
FN_Shell_NotifyIconW pShell_NotifyIconW = NULL;

FN_RegNotifyChangeKeyValue pRegNotifyChangeKeyValue = NULL;

extern "C" {

//////////////////////////////////////////////////////////////////////////////
// Task #3: Add new functions to hook

__declspec(dllexport)
INT WINAPI NewMessageBoxA(HWND hwnd, const char *text, const char *title, UINT uType)
{
    INT ret = 0;
    if (pMessageBoxA)
    {
        log_printf("MessageBoxA: enter: (%s, %s, %s, %u);\n", HWND2TEXT(hwnd), text, title, uType);
        ret = (*pMessageBoxA)(hwnd, "OK, Hooked", title, uType);
        log_printf("MessageBoxA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
INT WINAPI NewMessageBoxW(HWND hwnd, const WCHAR *text, const WCHAR *title, UINT uType)
{
    INT ret = 0;
    if (pMessageBoxW)
    {
        log_printf("MessageBoxW: enter: (%s, %ls, %ls, %u);\n", HWND2TEXT(hwnd), text, title, uType);
        ret = (*pMessageBoxW)(hwnd, L"OK, Hooked", title, uType);
        log_printf("MessageBoxW: leave: ret = %d;\n", ret);
    }
    return ret;
}

#define GET_MSG_TEXT(bUnicode) \
    md_dump(bUnicode, uMsg, wParam, lParam).c_str()

__declspec(dllexport)
BOOL WINAPI NewPostMessageA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret = 0;
    if (pPostMessageA)
    {
        log_printf("PostMessageA: enter: (%s, 0x%X, %p, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, GET_MSG_TEXT(FALSE));
        ret = (*pPostMessageA)(hWnd, uMsg, wParam, lParam);
        log_printf("PostMessageA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI NewPostMessageW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret = FALSE;
    if (pPostMessageW)
    {
        log_printf("PostMessageW: enter: (%s, 0x%X, %p, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, GET_MSG_TEXT(TRUE));
        ret = (*pPostMessageW)(hWnd, uMsg, wParam, lParam);
        log_printf("PostMessageW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
LRESULT WINAPI NewSendMessageA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = 0;
    if (pSendMessageA)
    {
        log_printf("SendMessageA: enter: (%s, 0x%X, %p, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, GET_MSG_TEXT(FALSE));
        ret = (*pSendMessageA)(hWnd, uMsg, wParam, lParam);
        log_printf("SendMessageA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
LRESULT WINAPI NewSendMessageW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret = FALSE;
    if (pSendMessageW)
    {
        log_printf("SendMessageW: enter: (%s, 0x%X, %p, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, GET_MSG_TEXT(TRUE));
        ret = (*pSendMessageW)(hWnd, uMsg, wParam, lParam);
        log_printf("SendMessageW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI NewSendNotifyMessageA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret = FALSE;
    if (pSendNotifyMessageA)
    {
        log_printf("SendNotifyMessageA: enter: (%s, 0x%X, %p, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, GET_MSG_TEXT(FALSE));
        ret = (*pSendNotifyMessageA)(hWnd, uMsg, wParam, lParam);
        log_printf("SendNotifyMessageA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI NewSendNotifyMessageW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL ret = FALSE;
    if (pSendNotifyMessageW)
    {
        log_printf("SendNotifyMessageW: enter: (%s, 0x%X, %p, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, GET_MSG_TEXT(TRUE));
        ret = (*pSendNotifyMessageW)(hWnd, uMsg, wParam, lParam);
        log_printf("SendNotifyMessageW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI
NewSendMessageCallbackA(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    SENDASYNCPROC fnCallback,
    ULONG_PTR dwData)
{
    BOOL ret = FALSE;
    if (pSendMessageCallbackA)
    {
        log_printf("SendMessageCallbackA: enter: (%s, 0x%X, %p, %p, %p, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, fnCallback, dwData, GET_MSG_TEXT(FALSE));
        ret = (*pSendMessageCallbackA)(hWnd, uMsg, wParam, lParam, fnCallback, dwData);
        log_printf("SendMessageCallbackA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI
NewSendMessageCallbackW(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    SENDASYNCPROC fnCallback,
    ULONG_PTR dwData)
{
    BOOL ret = FALSE;
    if (pSendMessageCallbackW)
    {
        log_printf("SendMessageCallbackW: enter: (%s, 0x%X, %p, %p, %p, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, fnCallback, dwData, GET_MSG_TEXT(TRUE));
        ret = (*pSendMessageCallbackW)(hWnd, uMsg, wParam, lParam, fnCallback, dwData);
        log_printf("SendMessageCallbackW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI
NewSendMessageTimeoutA(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    UINT fuFlags,
    UINT uTimeout,
    LPDWORD pdwResult)
{
    LRESULT ret = 0;
    if (pSendMessageTimeoutA)
    {
        log_printf("SendMessageTimeoutA: enter: (%s, 0x%X, %p, %p, %u, %u, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, fuFlags, uTimeout, pdwResult, GET_MSG_TEXT(FALSE));
        ret = (*pSendMessageTimeoutA)(hWnd, uMsg, wParam, lParam, fuFlags, uTimeout, pdwResult);
        log_printf("SendMessageTimeoutA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI
NewSendMessageTimeoutW(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    UINT fuFlags,
    UINT uTimeout,
    LPDWORD pdwResult)
{
    LRESULT ret = 0;
    if (pSendMessageTimeoutW)
    {
        log_printf("SendMessageTimeoutW: enter: (%s, 0x%X, %p, %p, %u, %u, %p): %s;\n", HWND2TEXT(hWnd), uMsg, wParam, lParam, fuFlags, uTimeout, pdwResult, GET_MSG_TEXT(TRUE));
        ret = (*pSendMessageTimeoutW)(hWnd, uMsg, wParam, lParam, fuFlags, uTimeout, pdwResult);
        log_printf("SendMessageTimeoutW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
INT WINAPI NewBroadcastSystemMessageA(DWORD dwFlags, LPDWORD lpdwRecipients, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT ret = 0;
    if (pBroadcastSystemMessageA)
    {
        log_printf("BroadcastSystemMessageA: enter: (0x%08lX, %p, 0x%X, %p, %p): %s;\n", dwFlags, lpdwRecipients, uMsg, wParam, lParam, GET_MSG_TEXT(FALSE));
        ret = (*pBroadcastSystemMessageA)(dwFlags, lpdwRecipients, uMsg, wParam, lParam);
        log_printf("BroadcastSystemMessageA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
INT WINAPI NewBroadcastSystemMessageW(DWORD dwFlags, LPDWORD lpdwRecipients, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    INT ret = 0;
    if (pBroadcastSystemMessageW)
    {
        log_printf("BroadcastSystemMessageW: enter: (0x%08lX, %p, 0x%X, %p, %p): %s;\n", dwFlags, lpdwRecipients, uMsg, wParam, lParam, GET_MSG_TEXT(TRUE));
        ret = (*pBroadcastSystemMessageW)(dwFlags, lpdwRecipients, uMsg, wParam, lParam);
        log_printf("BroadcastSystemMessageW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
void WINAPI NewNotifyWinEvent(
    DWORD event,
    HWND  hwnd,
    LONG  idObject,
    LONG  idChild)
{
    if (pNotifyWinEvent)
    {
        log_printf("NotifyWinEvent: enter: (0x%08lX, %s, %ld, %ld);\n", event, HWND2TEXT(hwnd), idObject, idChild);
        (*pNotifyWinEvent)(event, hwnd, idObject, idChild);
        log_printf("NotifyWinEvent: leave;\n");
    }
}

__declspec(dllexport)
HANDLE WINAPI
NewRegisterDeviceNotificationW(
    HANDLE hRecipient,
    LPVOID NotificationFilter,
    DWORD  Flags)
{
    HANDLE ret = NULL;
    if (pRegisterDeviceNotificationW)
    {
        log_printf("RegisterDeviceNotificationW: enter: (%p, %p, 0x%08lX);\n", hRecipient, NotificationFilter, Flags);
        ret = (*pRegisterDeviceNotificationW)(hRecipient, NotificationFilter, Flags);
        log_printf("RegisterDeviceNotificationW: leave: ret = %p;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL WINAPI
NewUnregisterDeviceNotification(HANDLE Handle)
{
    BOOL ret = FALSE;
    if (pUnregisterDeviceNotification)
    {
        log_printf("UnregisterDeviceNotification: enter: (%p);\n", Handle);
        ret = (*pUnregisterDeviceNotification)(Handle);
        log_printf("UnregisterDeviceNotification: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
LRESULT WINAPI
NewSendDlgItemMessageA(HWND hWnd, int nDlgItem, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG ret = FALSE;
    if (pSendDlgItemMessageA)
    {
        log_printf("SendDlgItemMessageA: enter: (%s, %d, 0x%X, %p, %p): %s;\n", HWND2TEXT(hWnd), nDlgItem, uMsg, wParam, lParam, GET_MSG_TEXT(FALSE));
        ret = (*pSendDlgItemMessageA)(hWnd, nDlgItem, uMsg, wParam, lParam);
        log_printf("SendDlgItemMessageA: leave: ret = 0x%08lX;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
LRESULT WINAPI
NewSendDlgItemMessageW(HWND hWnd, int nDlgItem, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG ret = FALSE;
    if (pSendDlgItemMessageW)
    {
        log_printf("SendDlgItemMessageW: enter: (%s, %d, 0x%X, %p, %p): %s;\n", HWND2TEXT(hWnd), nDlgItem, uMsg, wParam, lParam, GET_MSG_TEXT(TRUE));
        ret = (*pSendDlgItemMessageW)(hWnd, nDlgItem, uMsg, wParam, lParam);
        log_printf("SendDlgItemMessageW: leave: ret = 0x%08lX;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
void STDAPICALLTYPE
NewSHChangeNotify(LONG wEventId, UINT uFlags, LPCVOID dwItem1, LPCVOID dwItem2)
{
    if (pSHChangeNotify)
    {
        log_printf("SHChangeNotify: enter: (%ld, 0x%08lX, %p, %p);\n", wEventId, uFlags, dwItem1, dwItem2);
        (*pSHChangeNotify)(wEventId, uFlags, dwItem1, dwItem2);
        log_printf("SHChangeNotify: leave;\n");
    }
}

__declspec(dllexport)
HANDLE STDAPICALLTYPE
NewSHChangeNotification_Lock(
    HANDLE          hChange,
    DWORD           dwProcId,
    LPITEMIDLIST  **pppidl,
    LONG           *plEvent)
{
    HANDLE ret = NULL;
    if (pSHChangeNotification_Lock)
    {
        log_printf("SHChangeNotification_Lock: enter: (%p, 0x%08lX, %p, %p);\n", hChange, dwProcId, pppidl, plEvent);
        ret = (*pSHChangeNotification_Lock)(hChange, dwProcId, pppidl, plEvent);
        log_printf("SHChangeNotification_Lock: leave: ret = %p;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL STDAPICALLTYPE
NewSHChangeNotification_Unlock(HANDLE hLock)
{
    BOOL ret = FALSE;
    if (pSHChangeNotification_Unlock)
    {
        log_printf("SHChangeNotification_Unlock: enter: (%p);\n", hLock);
        ret = (*pSHChangeNotification_Unlock)(hLock);
        log_printf("SHChangeNotification_Unlock: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
ULONG STDAPICALLTYPE
NewSHChangeNotifyRegister(
    HWND     hwnd,
    int      fSources,
    LONG     fEvents,
    UINT     wMsg,
    int      cEntries,
    void    *pshcne)
{
    ULONG ret = 0;
    if (pSHChangeNotifyRegister)
    {
        log_printf("SHChangeNotifyRegister: enter: (%p, %d, %ld, %u, %d, %p);\n", hwnd, fSources, fEvents, wMsg, cEntries, pshcne);
        ret = (*pSHChangeNotifyRegister)(hwnd, fSources, fEvents, wMsg, cEntries, pshcne);
        log_printf("SHChangeNotifyRegister: leave: ret = 0x%08lX;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL STDAPICALLTYPE
NewSHChangeNotifyDeregister(ULONG ulID)
{
    BOOL ret = FALSE;
    if (pSHChangeNotifyDeregister)
    {
        log_printf("SHChangeNotifyDeregister: enter: (0x%08lX);\n", ulID);
        ret = (*pSHChangeNotifyDeregister)(ulID);
        log_printf("SHChangeNotifyDeregister: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL STDAPICALLTYPE
NewSHChangeNotifySuspendResume(BOOL b1, LPITEMIDLIST pidl, BOOL b2, LONG n)
{
    BOOL ret = FALSE;
    if (pSHChangeNotifySuspendResume)
    {
        log_printf("SHChangeNotifySuspendResume: enter: (%d, %p, %d, %ld);\n", b1, pidl, b2, n);
        ret = (*pSHChangeNotifySuspendResume)(b1, pidl, b2, n);
        log_printf("SHChangeNotifySuspendResume: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL STDAPICALLTYPE
NewShell_NotifyIconA(DWORD dwMessage, PNOTIFYICONDATAA lpdata)
{
    BOOL ret = FALSE;
    if (pShell_NotifyIconA)
    {
        log_printf("Shell_NotifyIconA: enter: (0x%08lX, %p);\n", dwMessage, lpdata);
        ret = (*pShell_NotifyIconA)(dwMessage, lpdata);
        log_printf("Shell_NotifyIconA: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
BOOL STDAPICALLTYPE
NewShell_NotifyIconW(DWORD dwMessage, PNOTIFYICONDATAW lpdata)
{
    BOOL ret = FALSE;
    if (pShell_NotifyIconW)
    {
        log_printf("Shell_NotifyIconW: enter: (0x%08lX, %p);\n", dwMessage, lpdata);
        ret = (*pShell_NotifyIconW)(dwMessage, lpdata);
        log_printf("Shell_NotifyIconW: leave: ret = %d;\n", ret);
    }
    return ret;
}

__declspec(dllexport)
LONG WINAPI NewRegNotifyChangeKeyValue(
    HKEY hKey,
    BOOL bWatchSubtree,
    DWORD dwNotifyFilter,
    HANDLE hEvent,
    BOOL fAsynchronous)
{
    LONG ret = 0;
    if (pRegNotifyChangeKeyValue)
    {
        log_printf("RegNotifyChangeKeyValue: enter: (%p, %d, 0x%08lX, %p, %d);\n", hKey, bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous);
        ret = (*pRegNotifyChangeKeyValue)(hKey, bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous);
        log_printf("RegNotifyChangeKeyValue: leave: ret = %ld;\n", ret);
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////////////

} // extern "C"

//////////////////////////////////////////////////////////////////////////////

FARPROC
ApiHookModule(HMODULE hMod, const char *dll_name,
              const char *fn_name, FARPROC fnNew, BOOL bDoHook)
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
        //log_printf("entry_dll_name: %s, dll_name: %s\n", entry_dll_name, dll_name);
        if (!_stricmp(entry_dll_name, dll_name))
            break;
    }
    if (!pImpDesc->Name)
    {
        //log_printf("ApiHookModule: !pImpDesc->Name\n");
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

            if (_stricmp((char *)pName->Name, fn_name) != 0)
                continue;
        }

        DWORD dwOldProtect, dwSize = sizeof(pIAT->u1.Function);
        if (VirtualProtect(&pIAT->u1.Function, dwSize, PAGE_READWRITE, &dwOldProtect))
        {
            FARPROC fnOld = (FARPROC)pIAT->u1.Function;
            pIAT->u1.Function = (DWORD_PTR)fnNew;
            VirtualProtect(&pIAT->u1.Function, dwSize, dwOldProtect, &dwOldProtect);

            if (bDoHook)
                log_printf("ApiHookModule: Hooked: %s\n", fn_name);
            else
                log_printf("ApiHookModule: Unhooked: %s\n", fn_name);
            return fnOld;
        }
    }

    //log_printf("ApiHookModule: %s: not found\n", fn_name);
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////
// Task #4: Add hooking/unhooking entries

struct HOOK_ENTRY
{
    const char *dll_file;
    const char *fn_name;
    FARPROC new_fn;
    FARPROC& old_fn;
};

#define BEGIN_HOOK_ENTRIES() \
    HOOK_ENTRY g_hook_entry[] = {

#define ADD_HOOK_ENTRY(dll_file, fn_name) \
        { dll_file, #fn_name, \
          reinterpret_cast<FARPROC>(New##fn_name), \
          reinterpret_cast<FARPROC&>(p##fn_name) },

#define END_HOOK_ENTRIES() \
    };

BEGIN_HOOK_ENTRIES()
    ADD_HOOK_ENTRY("user32.dll", MessageBoxA)
    ADD_HOOK_ENTRY("user32.dll", MessageBoxW)
    ADD_HOOK_ENTRY("user32.dll", PostMessageA)
    ADD_HOOK_ENTRY("user32.dll", PostMessageW)
    ADD_HOOK_ENTRY("user32.dll", SendMessageA)
    ADD_HOOK_ENTRY("user32.dll", SendMessageW)
    ADD_HOOK_ENTRY("user32.dll", SendNotifyMessageA)
    ADD_HOOK_ENTRY("user32.dll", SendNotifyMessageW)
    ADD_HOOK_ENTRY("user32.dll", SendMessageCallbackA)
    ADD_HOOK_ENTRY("user32.dll", SendMessageCallbackW)
    ADD_HOOK_ENTRY("user32.dll", SendMessageTimeoutA)
    ADD_HOOK_ENTRY("user32.dll", SendMessageTimeoutW)
    ADD_HOOK_ENTRY("user32.dll", BroadcastSystemMessageA)
    ADD_HOOK_ENTRY("user32.dll", BroadcastSystemMessageW)
    ADD_HOOK_ENTRY("user32.dll", NotifyWinEvent)
    ADD_HOOK_ENTRY("user32.dll", RegisterDeviceNotificationW)
    ADD_HOOK_ENTRY("user32.dll", UnregisterDeviceNotification)
    ADD_HOOK_ENTRY("user32.dll", SendDlgItemMessageA)
    ADD_HOOK_ENTRY("user32.dll", SendDlgItemMessageW)
    ADD_HOOK_ENTRY("shell32.dll", SHChangeNotify)
    ADD_HOOK_ENTRY("shell32.dll", SHChangeNotification_Lock)
    ADD_HOOK_ENTRY("shell32.dll", SHChangeNotification_Unlock)
    ADD_HOOK_ENTRY("shell32.dll", SHChangeNotifyRegister)
    ADD_HOOK_ENTRY("shell32.dll", SHChangeNotifyDeregister)
    ADD_HOOK_ENTRY("shell32.dll", SHChangeNotifySuspendResume)
    ADD_HOOK_ENTRY("shell32.dll", Shell_NotifyIconA)
    ADD_HOOK_ENTRY("shell32.dll", Shell_NotifyIconW)
    ADD_HOOK_ENTRY("advapi32.dll", RegNotifyChangeKeyValue)
END_HOOK_ENTRIES()

//////////////////////////////////////////////////////////////////////////////

static BOOL hook(void)
{
    for (size_t i = 0; i < _countof(g_hook_entry); ++i)
    {
        HOOK_ENTRY *entry = &g_hook_entry[i];
        entry->old_fn = ApiHookModule(NULL, entry->dll_file, entry->fn_name, entry->new_fn, TRUE);
    }
    return TRUE;
}

static void unhook(void)
{
    for (size_t i = 0; i < _countof(g_hook_entry); ++i)
    {
        HOOK_ENTRY *entry = &g_hook_entry[i];
        ApiHookModule(NULL, entry->dll_file, entry->fn_name, (FARPROC)entry->old_fn, FALSE);
        entry->old_fn = NULL;
    }
}

void show_window_info(const char *name, HWND hwnd)
{
    DWORD pid, tid;
    tid = GetWindowThreadProcessId(hwnd, &pid);
    log_printf("<%s> HWND: %p, PID: 0x%08lX, TID: 0x%08lX\n", name, hwnd, pid, tid);
}

BOOL CALLBACK EnumExplorerProc(HWND hwnd, LPARAM lParam)
{
    if (ExplorerWndCount >= MAX_EXPLORER)
        return FALSE;   // stop

    TCHAR szClass[128];
    GetClassName(hwnd, szClass, 128);
    if (lstrcmpi(szClass, TEXT("CabinetWClass")) == 0)
    {
        HWND CabinetWClass = hwnd;
        show_window_info("CabinetWClass", CabinetWClass);
        ExplorerWnd[ExplorerWndCount++] = CabinetWClass;
    }
    else if (lstrcmpi(szClass, TEXT("ExploreWClass")) == 0)
    {
        HWND ExploreWClass = hwnd;
        show_window_info("ExploreWClass", ExploreWClass);
        ExplorerWnd[ExplorerWndCount++] = ExploreWClass;
    }

    return TRUE;    // continue
}

void show_info()
{
    log_printf("### SysNotifyHooker 2017.12.11 by ReactOS Team ###\n");

    Shell_TrayWnd = FindWindow(TEXT("Shell_TrayWnd"), NULL);
    show_window_info("Shell_TrayWnd", Shell_TrayWnd);

    StartButton = FindWindowEx(Shell_TrayWnd, NULL, TEXT("BUTTON"), NULL);
    if (StartButton == NULL)
        StartButton = FindWindowEx(Shell_TrayWnd, NULL, TEXT("Start"), NULL);
    show_window_info("StartButton", StartButton);

    TrayNotifyWnd = FindWindowEx(Shell_TrayWnd, NULL, TEXT("TrayNotifyWnd"), NULL);
    show_window_info("TrayNotifyWnd", TrayNotifyWnd);

    TrayClockWClass = FindWindowEx(TrayNotifyWnd, NULL, TEXT("TrayClockWClass"), NULL);
    show_window_info("TrayClockWClass", TrayClockWClass);

    Progman = FindWindow(TEXT("Progman"), NULL);
    show_window_info("Progman", Progman);

    EnumWindows(EnumExplorerProc, 0);
}

extern "C"
BOOL WINAPI
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        g_hinstDLL = hinstDLL;
        log_printf("DLL_PROCESS_ATTACH: %p\n", hinstDLL);
        show_info();
        hook();
        break;
    case DLL_PROCESS_DETACH:
        log_printf("DLL_PROCESS_DETACH: %p\n", hinstDLL);
        unhook();
        break;
    case DLL_THREAD_ATTACH:
        log_printf("DLL_THREAD_ATTACH: %p\n", hinstDLL);
        break;
    case DLL_THREAD_DETACH:
        log_printf("DLL_THREAD_DETACH: %p\n", hinstDLL);
        break;
    }
    return TRUE;
}
