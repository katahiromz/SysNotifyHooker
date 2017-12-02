// targetprog.cpp --- the target program
//////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <windowsx.h>   // for HANDLE_MSG
#include <tchar.h>
#include <dlgs.h>       // for psh1

//////////////////////////////////////////////////////////////////////////////

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    DWORD dwProcessID = GetCurrentProcessId();
    SetDlgItemInt(hwnd, edt1, (UINT)dwProcessID, FALSE);
    return TRUE;
}

VOID OnTest(HWND hwnd)
{
    MessageBoxA(hwnd, "Not hooked yet!", "OnTest", MB_ICONINFORMATION);
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDOK:
        EndDialog(hwnd, IDOK);
        break;
    case IDCANCEL:
        EndDialog(hwnd, IDCANCEL);
        break;
    case psh1:
        OnTest(hwnd);
        break;
    }
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////

extern "C"
INT APIENTRY _tWinMain(
    HINSTANCE   hInstance,
    HINSTANCE   hPrevInstance,
    LPTSTR      lpCmdLine,
    INT         nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////
