#include "winxx.h"
#include <string>

std::string *
md_hwnd2string(HWND hwnd)
{
    return reinterpret_cast<std::string *>(hwnd);
}

/****************************************************************************/
/* message details */

LRESULT
md_OnUnknown(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT
md_OnUser(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char buf[512];
    wsprintfA(buf, "WM_USER+%u()", uMsg - WM_USER);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

LRESULT
md_OnApp(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char buf[512];
    wsprintfA(buf, "WM_APP+%u()", uMsg - WM_APP);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

LRESULT
md_OnNull(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_NULL()";
}

BOOL
md_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    char buf[512];
    wsprintfA(buf, "WM_CREATE(%p)", lpCreateStruct);
    *md_hwnd2string(hwnd) = buf;
    return TRUE;
}

void md_OnDestroy(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_DESTROY()";
}

void md_OnMove(HWND hwnd, int x, int y)
{
    char buf[512];
    wsprintfA(buf, "WM_MOVE(%d, %d)", x, y);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    char buf[512];
    wsprintfA(buf, "WM_SIZE(%u, %d, %d)", state, cx, cy);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
    char buf[512];
    wsprintfA(buf, "WM_ACTIVATE(%u, %p, %d)", state, hwndActDeact, fMinimized);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSetFocus(HWND hwnd, HWND hwndOldFocus)
{
    char buf[512];
    wsprintfA(buf, "WM_SETFOCUS(%p)", hwndOldFocus);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnKillFocus(HWND hwnd, HWND hwndNewFocus)
{
    char buf[512];
    wsprintfA(buf, "WM_KILLFOCUS(%p)", hwndNewFocus);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnEnable(HWND hwnd, BOOL fEnable)
{
    char buf[512];
    wsprintfA(buf, "WM_ENABLE(%d)", fEnable);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSetRedraw(HWND hwnd, BOOL fRedraw)
{
    char buf[512];
    wsprintfA(buf, "WM_SETREDRAW(%d)", fRedraw);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSetText(HWND hwnd, LPCTSTR lpszText)
{
    char buf[512];
    #ifdef UNICODE
        wsprintfA(buf, "WM_SETTEXT(%ls)", lpszText);
    #else
        wsprintfA(buf, "WM_SETTEXT(%s)", lpszText);
    #endif
    *md_hwnd2string(hwnd) = buf;
}

INT md_OnGetText(HWND hwnd, int cchTextMax, LPTSTR lpszText)
{
    char buf[512];
    wsprintfA(buf, "WM_GETTEXT(%d, %p)", cchTextMax, lpszText);
    *md_hwnd2string(hwnd) = buf;
}

INT md_OnGetTextLength(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_GETTEXTLENGTH()";
}

void md_OnPaint(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_PAINT()";
}

void md_OnClose(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_CLOSE()";
}

BOOL md_OnQueryEndSession(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_QUERYENDSESSION()";
    return FALSE;
}

BOOL md_OnQueryOpen(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_QUERYOPEN()";
    return FALSE;
}

void md_OnEndSession(HWND hwnd, BOOL fEnding)
{
    char buf[512];
    wsprintfA(buf, "WM_ENDSESSION(%d)", fEnding);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnQuit(HWND hwnd, int exitCode)
{
    char buf[512];
    wsprintfA(buf, "WM_QUIT(%d)", exitCode);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnEraseBkgnd(HWND hwnd, HDC hdc)
{
    char buf[512];
    wsprintfA(buf, "WM_ERASEBKGND(%p)", hdc);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSysColorChange(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_SYSCOLORCHANGE()";
}

void md_OnShowWindow(HWND hwnd, BOOL fShow, UINT status)
{
    char buf[512];
    wsprintfA(buf, "WM_SHOWWINDOW(%d, %u)", fShow, status);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnWinIniChange(HWND hwnd, LPCTSTR lpszSectionName)
{
    char buf[512];
    #ifdef UNICODE
        wsprintfA(buf, "WM_WININICHANGE(%ls)", lpszSectionName);
    #else
        wsprintfA(buf, "WM_WININICHANGE(%s)", lpszSectionName);
    #endif
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSettingChange(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    *md_hwnd2string(hwnd) = "WM_SETTINGCHANGE()";
}

void md_OnDevModeChange(HWND hwnd, LPCTSTR lpszDeviceName)
{
    char buf[512];
    #ifdef UNICODE
        wsprintfA(buf, "WM_DEVMODECHANGE(%s)", lpszDeviceName);
    #else
        wsprintfA(buf, "WM_DEVMODECHANGE(%ls)", lpszDeviceName);
    #endif
    *md_hwnd2string(hwnd) = buf;
}

void md_OnActivateApp(HWND hwnd, BOOL fActivate, DWORD dwThreadId)
{
    char buf[512];
    wsprintfA(buf, "WM_ACTIVATEAPP(%d, 0x%08lX)", fActivate, dwThreadId);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnFontChange(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_FONTCHANGE()";
}

void md_OnTimeChange(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_TIMECHANGE()";
}

void md_OnCancelMode(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_CANCELMODE()";
}

BOOL md_OnSetCursor(HWND hwnd, HWND hwndCursor, UINT codeHitTest, UINT msg)
{
    char buf[512];
    wsprintfA(buf, "WM_SETCURSOR(%p, %u, %u)", hwndCursor, codeHitTest, msg);
    *md_hwnd2string(hwnd) = buf;
    return FALSE;
}

int md_OnMouseActivate(HWND hwnd, HWND hwndTopLevel, UINT codeHitTest, UINT msg)
{
    char buf[512];
    wsprintfA(buf, "WM_MOUSEACTIVATE(%p, %u, %u)", hwndTopLevel, codeHitTest, msg);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

void md_OnChildActivate(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_CHILDACTIVATE()";
}

void md_OnQueueSync(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_QUEUESYNC()";
}

void md_OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
    char buf[512];
    wsprintfA(buf, "WM_GETMINMAXINFO(%p)", lpMinMaxInfo);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnIconEraseBkgnd(HWND hwnd, HDC hdc)
{
    char buf[512];
    wsprintfA(buf, "WM_ICONERASEBKGND(%p)", hdc);
    *md_hwnd2string(hwnd) = buf;
    return FALSE;
}

HWND md_OnNextDlgCtl(HWND hwnd, HWND hwndSetFocus, BOOL fNext)
{
    char buf[512];
    wsprintfA(buf, "WM_NEXTDLGCTL(%p, %d)", hwndSetFocus, fNext);
    *md_hwnd2string(hwnd) = buf;
    return NULL;
}

void md_OnSpoolerStatus(HWND hwnd, UINT status, int cJobInQueue)
{
    char buf[512];
    wsprintfA(buf, "WM_SPOOLERSTATUS(%u, %d)", status, cJobInQueue);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{
    char buf[512];
    wsprintfA(buf, "WM_DRAWITEM(%p)", lpDrawItem);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMeasureItem(HWND hwnd, MEASUREITEMSTRUCT * lpMeasureItem)
{
    char buf[512];
    wsprintfA(buf, "WM_MEASUREITEM(%p)", lpMeasureItem);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnDeleteItem(HWND hwnd, const DELETEITEMSTRUCT * lpDeleteItem)
{
    char buf[512];
    wsprintfA(buf, "WM_DELETEITEM(%p)", lpDeleteItem);
    *md_hwnd2string(hwnd) = buf;
}

int md_OnVkeyToItem(HWND hwnd, UINT vk, HWND hwndListbox, int iCaret)
{
    char buf[512];
    wsprintfA(buf, "WM_VKEYTOITEM(%u, %p, iCaret)", vk, hwndListbox, iCaret);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

int md_OnCharToItem(HWND hwnd, UINT ch, HWND hwndListbox, int iCaret)
{
    char buf[512];
    wsprintfA(buf, "WM_CHARTOITEM(%u, %p, iCaret)", ch, hwndListbox, iCaret);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSetFont(HWND hwndCtl, HFONT hfont, BOOL fRedraw)
{
    char buf[512];
    wsprintfA(buf, "WM_SETFONT(%p, %d)", hfont, fRedraw);
    *md_hwnd2string(hwndCtl) = buf;
}

HFONT md_OnGetFont(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_GETFONT()";
    return NULL;
}

HICON md_OnQueryDragIcon(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_QUERYDRAGICON()";
    return NULL;
}

int md_OnCompareItem(HWND hwnd, const COMPAREITEMSTRUCT * lpCompareItem)
{
    char buf[512];
    wsprintfA(buf, "WM_COMPAREITEM(%p)", lpCompareItem);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

void md_OnCompacting(HWND hwnd, UINT compactRatio)
{
    char buf[512];
    wsprintfA(buf, "WM_COMPACTING(%u)", compactRatio);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnCommNotify(HWND hwnd, int cid, UINT flags)
{
    char buf[512];
    wsprintfA(buf, "WM_COMMNOTIFY(%d, %u)", cid, flags);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnWindowPosChanging(HWND hwnd, LPWINDOWPOS lpwpos)
{
    char buf[512];
    wsprintfA(buf, "WM_WINDOWPOSCHANGING(%p)", lpwpos);
    *md_hwnd2string(hwnd) = buf;
    return FALSE;
}

void md_OnWindowPosChanged(HWND hwnd, const LPWINDOWPOS lpwpos)
{
    char buf[512];
    wsprintfA(buf, "WM_WINDOWPOSCHANGED(%p)", lpwpos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnPower(HWND hwnd, int code)
{
    char buf[512];
    wsprintfA(buf, "WM_POWER(%d)", code);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnCopyData(HWND hwnd, HWND hwndFrom, PCOPYDATASTRUCT pcds)
{
    char buf[512];
    wsprintfA(buf, "WM_COPYDATA(%p, %p)", hwndFrom, pcds);
    *md_hwnd2string(hwnd) = buf;
    return FALSE;
}

LRESULT md_OnNotify(HWND hwnd, int idFrom, LPNMHDR pnmhdr)
{
    char buf[512];
    wsprintfA(buf, "WM_NOTIFY(%d, %p)", idFrom, pnmhdr);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

void md_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos)
{
    char buf[512];
    wsprintfA(buf, "WM_CONTEXTMENU(%p, %u, %u)", hwndContext, xPos, yPos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnDisplayChange(HWND hwnd, UINT bitsPerPixel, UINT cxScreen, UINT cyScreen)
{
    char buf[512];
    wsprintfA(buf, "WM_DISPLAYCHANGE(%u, %u, %u)", bitsPerPixel, cxScreen, cyScreen);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnNCCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    char buf[512];
    wsprintfA(buf, "WM_NCCREATE(%p)", lpCreateStruct);
    *md_hwnd2string(hwnd) = buf;
    return FALSE;
}

void md_OnNCDestroy(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_NCDESTROY()";
}

UINT md_OnNCCalcSize(HWND hwnd, BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp)
{
    char buf[512];
    wsprintfA(buf, "WM_NCCALCSIZE(%d, %p)", fCalcValidRects, lpcsp);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

UINT md_OnNCHitTest(HWND hwnd, int x, int y)
{
    char buf[512];
    wsprintfA(buf, "WM_NCHITTEST(%d, %d)", x, y);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

void md_OnNCPaint(HWND hwnd, HRGN hrgn)
{
    char buf[512];
    wsprintfA(buf, "WM_NCPAINT(%p)", hrgn);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnNCActivate(HWND hwnd, BOOL fActive, HWND hwndActDeact, BOOL fMinimized)
{
    char buf[512];
    wsprintfA(buf, "WM_NCACTIVATE(%d, %p, %d)", fActive, hwndActDeact, fMinimized);
    *md_hwnd2string(hwnd) = buf;
    return FALSE;
}

UINT md_OnGetDlgCode(HWND hwnd, LPMSG lpmsg)
{
    char buf[512];
    wsprintfA(buf, "WM_GETDLGCODE(%p)", lpmsg);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNCMouseMove(HWND hwnd, int x, int y, UINT codeHitTest)
{
    char buf[512];
    wsprintfA(buf, "WM_NCMOUSEMOVE(%d, %d, %u)", x, y, codeHitTest);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNCLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest)
{
    char buf[512];
    wsprintfA(buf, "WM_NCLBUTTONDOWN(%d, %d, %d, %u)", fDoubleClick, x, y, codeHitTest);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNCLButtonUp(HWND hwnd, int x, int y, UINT codeHitTest)
{
    char buf[512];
    wsprintfA(buf, "WM_NCLBUTTONUP(%d, %d, %u)", x, y, codeHitTest);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNCRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest)
{
    char buf[512];
    wsprintfA(buf, "WM_NCRBUTTONDOWN(%d, %d, %d, %u)", fDoubleClick, x, y, codeHitTest);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNCRButtonUp(HWND hwnd, int x, int y, UINT codeHitTest)
{
    char buf[512];
    wsprintfA(buf, "WM_NCRBUTTONUP(%d, %d, %u)", x, y, codeHitTest);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNCMButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT codeHitTest)
{
    char buf[512];
    wsprintfA(buf, "WM_NCMBUTTONDOWN(%d, %d, %d, %u)", fDoubleClick, x, y, codeHitTest);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNCMButtonUp(HWND hwnd, int x, int y, UINT codeHitTest)
{
    char buf[512];
    wsprintfA(buf, "WM_NCMBUTTONUP(%d, %d, %u)", x, y, codeHitTest);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    char buf[512];
    if (fDown)
        wsprintfA(buf, "WM_KEYDOWN(%u, %d, %d, %u)", vk, fDown, cRepeat, flags);
    else
        wsprintfA(buf, "WM_KEYUP(%d, %d, %d, %u)", vk, fDown, cRepeat, flags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    char buf[512];
    wsprintfA(buf, "WM_CHAR(%u, %d)", ch, cRepeat);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnDeadChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    char buf[512];
    wsprintfA(buf, "WM_DEADCHAR(%u, %d)", ch, cRepeat);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSysKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    char buf[512];
    if (fDown)
        wsprintfA(buf, "WM_SYSKEYDOWN(%u, %d, %d, %u)", vk, fDown, cRepeat, flags);
    else
        wsprintfA(buf, "WM_SYSKEYUP(%u, %d, %d, %u)", vk, fDown, cRepeat, flags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSysChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    char buf[512];
    wsprintfA(buf, "WM_SYSCHAR(%u, %d)", ch, cRepeat);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSysDeadChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    char buf[512];
    wsprintfA(buf, "WM_SYSDEADCHAR(%u, %d)", ch, cRepeat);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    char buf[512];
    wsprintfA(buf, "WM_INITDIALOG(%p, %p)", hwndFocus, lParam);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    char buf[512];
    wsprintfA(buf, "WM_COMMAND(%d, %p, %u)", id, hwndCtl, codeNotify);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
    char buf[512];
    wsprintfA(buf, "WM_SYSCOMMAND(%u, %d, %d)", cmd, x, y);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnTimer(HWND hwnd, UINT id)
{
    char buf[512];
    wsprintfA(buf, "WM_TIMER(%u)", id);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
    char buf[512];
    wsprintfA(buf, "WM_HSCROLL(%p, %u, %d)", hwndCtl, code, pos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
    char buf[512];
    wsprintfA(buf, "WM_VSCROLL(%p, %u, %d)", hwndCtl, code, pos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnInitMenu(HWND hwnd, HMENU hMenu)
{
    char buf[512];
    wsprintfA(buf, "WM_INITMENU(%p)", hMenu);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu)
{
    char buf[512];
    wsprintfA(buf, "WM_INITMENUPOPUP(%p, %u, %d)", hMenu, item, fSystemMenu);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
    char buf[512];
    wsprintfA(buf, "WM_MENUSELECT(%p, %d, %p, %u)", hmenu, item, hmenuPopup, flags);
    *md_hwnd2string(hwnd) = buf;
}

DWORD md_OnMenuChar(HWND hwnd, UINT ch, UINT flags, HMENU hmenu)
{
    char buf[512];
    wsprintfA(buf, "WM_MENUCHAR(%u, %u, %p)", ch, flags, hmenu);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

void md_OnEnterIdle(HWND hwnd, UINT source, HWND hwndSource)
{
    char buf[512];
    wsprintfA(buf, "WM_ENTERIDLE(%u, %p)", source, hwndSource);
    *md_hwnd2string(hwnd) = buf;
}

HBRUSH md_OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
    char buf[512];
    wsprintfA(buf, "WM_CTLCOLOR(%p, %p, %d)", hdc, hwndChild, type);
    *md_hwnd2string(hwnd) = buf;
    return NULL;
}

void md_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    char buf[512];
    wsprintfA(buf, "WM_MOUSEMOVE(%d, %d, %u)", x, y, keyFlags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    char buf[512];
    wsprintfA(buf, "WM_LBUTTONDOWN(%d, %d, %d, %u)", fDoubleClick, x, y, keyFlags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    char buf[512];
    wsprintfA(buf, "WM_LBUTTONUP(%d, %d, %u)", x, y, keyFlags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    char buf[512];
    wsprintfA(buf, "WM_RBUTTONDOWN(%d, %d, %d, %u)", fDoubleClick, x, y, keyFlags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnRButtonUp(HWND hwnd, int x, int y, UINT flags)
{
    char buf[512];
    wsprintfA(buf, "WM_RBUTTONUP(%d, %d, %u)", x, y, flags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    char buf[512];
    wsprintfA(buf, "WM_MBUTTONDOWN(%d, %d, %d, %u)", fDoubleClick, x, y, keyFlags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMButtonUp(HWND hwnd, int x, int y, UINT flags)
{
    char buf[512];
    wsprintfA(buf, "WM_MBUTTONUP(%d, %d, %u)", x, y, flags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys)
{
    char buf[512];
    wsprintfA(buf, "WM_MOUSEWHEEL(%d, %d, %d, %u)", xPos, yPos, zDelta, fwKeys);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnParentNotify(HWND hwnd, UINT msg, HWND hwndChild, int idChild)
{
    char buf[512];
    wsprintfA(buf, "WM_PARENTNOTIFY(%u, %p, %d)", msg, hwndChild, idChild);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnDeviceChange(HWND hwnd, UINT uEvent, DWORD dwEventData)
{
    char buf[512];
    wsprintfA(buf, "WM_DEVICECHANGE(%u, 0x%08lX)", uEvent, dwEventData);
    *md_hwnd2string(hwnd) = buf;
    return FALSE;
}

HWND md_MDICreate(HWND hwnd, const LPMDICREATESTRUCT lpmcs)
{
    char buf[512];
    wsprintfA(buf, "WM_MDICREATE(%p)", lpmcs);
    *md_hwnd2string(hwnd) = buf;
    return NULL;
}

void md_MDIDestroy(HWND hwnd, HWND hwndDestroy)
{
    char buf[512];
    wsprintfA(buf, "WM_MDIDESTROY(%p)", hwndDestroy);
    *md_hwnd2string(hwnd) = buf;
}

void md_MDIActivate(HWND hwnd, BOOL fActive, HWND hwndActivate, HWND hwndDeactivate)
{
    char buf[512];
    wsprintfA(buf, "WM_MDIACTIVATE(%d, %p, %p)", fActive, hwndActivate, hwndDeactivate);
    *md_hwnd2string(hwnd) = buf;
}

void md_MDIRestore(HWND hwnd, HWND hwndRestore)
{
    char buf[512];
    wsprintfA(buf, "WM_MDIRESTORE(%p)", hwndRestore);
    *md_hwnd2string(hwnd) = buf;
}

HWND md_MDINext(HWND hwnd, HWND hwndCur, BOOL fPrev)
{
    char buf[512];
    wsprintfA(buf, "WM_MDINEXT(%p, %d)", hwndCur, fPrev);
    *md_hwnd2string(hwnd) = buf;
    return NULL;
}

void md_MDIMaximize(HWND hwnd, HWND hwndMaximize)
{
    char buf[512];
    wsprintfA(buf, "WM_MDIMAXIMIZE(%p)", hwndMaximize);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_MDITile(HWND hwnd, UINT cmd)
{
    char buf[512];
    wsprintfA(buf, "WM_MDITILE(%u)", cmd);
    *md_hwnd2string(hwnd) = buf;
    return FALSE;
}

BOOL md_MDICascade(HWND hwnd, UINT cmd)
{
    char buf[512];
    wsprintfA(buf, "WM_MDICASCADE(%u)", cmd);
    *md_hwnd2string(hwnd) = buf;
    return FALSE;
}

void md_MDIIconArrange(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_MDIICONARRANGE()";
}

HWND md_MDIGetActive(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_MDIGETACTIVE()";
}

HMENUmd_MDISetMenu(HWND hwnd, BOOL fRefresh, HMENU hmenuFrame, HMENU hmenuWindow)
{
    char buf[512];
    wsprintfA(buf, "WM_MDISETMENU(%d, %p, %p)", fRefresh, hmenuFrame, hmenuWindow);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnDropFiles(HWND hwnd, HDROP hdrop)
{
    char buf[512];
    wsprintfA(buf, "WM_DROPFILES(%p)", hdrop);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnCut(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_CUT()";
}

void md_OnCopy(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_COPY()";
}

void md_OnPaste(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_PASTE()";
}

void md_OnClear(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_CLEAR()";
}

void md_OnUndo(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_UNDO()";
}

HANDLE md_OnRenderFormat(HWND hwnd, UINT fmt)
{
    char buf[512];
    wsprintfA(buf, "WM_RENDERFORMAT(%u)", fmt);
    *md_hwnd2string(hwnd) = buf;
    return NULL;
}

void md_OnRenderAllFormats(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_RENDERALLFORMATS()";
}

void md_OnDestroyClipboard(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_DESTROYCLIPBOARD()";
}

void md_OnDrawClipboard(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_DRAWCLIPBOARD()";
}

void md_OnPaintClipboard(HWND hwnd, HWND hwndCBViewer, const LPPAINTSTRUCT lpPaintStruct)
{
    char buf[512];
    wsprintfA(buf, "WM_PAINTCLIPBOARD(%p, %p)", hwndCBViewer, lpPaintStruct);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnVScrollClipboard(HWND hwnd, HWND hwndCBViewer, UINT code, int pos)
{
    char buf[512];
    wsprintfA(buf, "WM_VSCROLLCLIPBOARD(%p, %u, %d)", hwndCBViewer, code, pos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSizeClipboard(HWND hwnd, HWND hwndCBViewer, const LPRECT lprc)
{
    char buf[512];
    wsprintfA(buf, "WM_SIZECLIPBOARD(%p, %p)", hwndCBViewer, lprc);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnAskCBFormatName(HWND hwnd, int cchMax, LPTSTR rgchName)
{
    char buf[512];
    wsprintfA(buf, "WM_ASKCBFORMATNAME(%d, %p)", cchMax, rgchName);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnChangeCBChain(HWND hwnd, HWND hwndRemove, HWND hwndNext)
{
    char buf[512];
    wsprintfA(buf, "WM_CHANGECBCHAIN(%p, %p)", hwndRemove, hwndNext);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnHScrollClipboard(HWND hwnd, HWND hwndCBViewer, UINT code, int pos)
{
    char buf[512];
    wsprintfA(buf, "WM_HSCROLLCLIPBOARD(%p, %u, %d)", hwndCBViewer, code, pos);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnQueryNewPalette(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_QUERYNEWPALETTE()";
}

void md_OnPaletteIsChanging(HWND hwnd, HWND hwndPaletteChange)
{
    char buf[512];
    wsprintfA(buf, "WM_PALETTEISCHANGING(%p)", hwndPaletteChange);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnPaletteChanged(HWND hwnd, HWND hwndPaletteChange)
{
    char buf[512];
    wsprintfA(buf, "WM_PALETTECHANGED(%p)", hwndPaletteChange);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnHotKey(HWND hwnd, int idHotKey, UINT fuModifiers, UINT vk)
{
    char buf[512];
    wsprintfA(buf, "WM_HOTKEY(%d, %u, %u)", idHotKey, fuModifiers, vk);
    *md_hwnd2string(hwnd) = buf;
}

/****************************************************************************/

INT md_OnSetHotKey(HWND hwnd, INT nCode, UINT nOptions)
{
    char buf[512];
    wsprintfA(buf, "WM_SETHOTKEY(%d, %u)", nCode, nOptions);
    *md_hwnd2string(hwnd) = buf;
}

INT md_OnGetHotKey(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_GETHOTKEY()";
}

void md_OnPaintIcon(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_PAINTICON()";
}

LRESULT md_OnGetObject(HWND hwnd, WPARAM wParam, DWORD dwObjId)
{
    char buf[512];
    wsprintfA(buf, "WM_GETOBJECT(%p, 0x%08lX)", wParam, dwObjId);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

void md_OnCancelJournal(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_CANCELJOURNAL()";
}

void md_OnInputLangChangeRequest(HWND hwnd, BOOL bFlag, HKL hKL)
{
    char buf[512];
    wsprintfA(buf, "WM_INPUTLANGCHANGEREQUEST(%d, %p)", bFlag, hKL);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnInputLangChange(HWND hwnd, DWORD dwCharSet, HKL hKL)
{
    char buf[512];
    wsprintfA(buf, "WM_INPUTLANGCHANGE(0x%08lX, %p)", dwCharSet, hKL);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnTCard(HWND hwnd, UINT idAction, DWORD dwActionData)
{
    char buf[512];
    wsprintfA(buf, "WM_TCARD(%u, 0x%08lX)", idAction, dwActionData);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnHelp(HWND hwnd, LPHELPINFO lpHelpInfo)
{
    char buf[512];
    wsprintfA(buf, "WM_HELP(%p)", lpHelpInfo);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnUserChanged(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_USERCHANGED()";
}

INT md_OnNotifyFormat(HWND hwnd, HWND hwndTarget, INT nCommand)
{
    char buf[512];
    wsprintfA(buf, "WM_NOTIFYFORMAT(%p, %d)", hwndTarget, nCommand);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnStyleChanging(HWND hwnd, UINT nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
    char buf[512];
    wsprintfA(buf, "WM_STYLECHANGING(%u, %p)", nStyleType, lpStyleStruct);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnStyleChanged(HWND hwnd, UINT nStyleType, const STYLESTRUCT *lpStyleStruct)
{
    char buf[512];
    wsprintfA(buf, "WM_STYLECHANGED(%u, %p)", nStyleType, lpStyleStruct);
    *md_hwnd2string(hwnd) = buf;
}

HICON md_OnGetIcon(HWND hwnd, UINT nType, LPARAM dpi)
{
    char buf[512];
    wsprintfA(buf, "WM_GETICON(%u, %p)", nType, dpi);
    *md_hwnd2string(hwnd) = buf;
}

HICON md_OnSetIcon(HWND hwnd, UINT nType, HICON hIcon)
{
    char buf[512];
    wsprintfA(buf, "WM_SETICON(%u, %p)", nType, hIcon);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSyncPaint(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_SYNCPAINT()";
}

void md_OnNCXButtonDown(HWND hwnd, BOOL fDoubleClick, UINT nHitTest, WORD fwButton, INT xPos, INT yPos)
{
    char buf[512];
    if (fDoubleClick)
        wsprintfA(buf, "WM_NCXBUTTONDBLCLK(%d, %u, %u, %d, %d)", fDoubleClick, nHitTest, fwButton, xPos, yPos);
    else
        wsprintfA(buf, "WM_NCXBUTTONDOWN(%d, %u, %u, %d, %d)", fDoubleClick, nHitTest, fwButton, xPos, yPos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNCXButtonUp(HWND hwnd, UINT nHitTest, WORD fwButton, INT xPos, INT yPos)
{
    char buf[512];
    wsprintfA(buf, "WM_NCXBUTTONUP(%u, %u, %d, %d)", nHitTest, fwButton, xPos, yPos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnImeStartComposition(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_IME_STARTCOMPOSITION()";
}

void md_OnImeEndComposition(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_IME_ENDCOMPOSITION()";
}

void md_OnImeComposition(HWND hwnd, WORD wChar, DWORD lAttribute)
{
    char buf[512];
    wsprintfA(buf, "WM_IME_COMPOSITION(%u, 0x%08lX)", wChar, lAttribute);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMenuRButtonUp(HWND hwnd, UINT nPos, HMENU hMenu)
{
    char buf[512];
    wsprintfA(buf, "WM_MENURBUTTONUP(%u, %p)", nPos, hMenu);
    *md_hwnd2string(hwnd) = buf;
}

UINT md_OnMenuDrag(HWND hwnd, UINT nPos, HMENU hMenu)
{
    char buf[512];
    wsprintfA(buf, "WM_MENUDRAG(%u, %p)", nPos, hMenu);
    *md_hwnd2string(hwnd) = buf;
}

UINT md_OnMenuGetObject(HWND hwnd, MENUGETOBJECTINFO *pmgoi)
{
    char buf[512];
    wsprintfA(buf, "WM_MENUGETOBJECT(%p)", pmgoi);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnUninitMenuPopup(HWND hwnd, HMENU hMenu, UINT nFlags)
{
    char buf[512];
    wsprintfA(buf, "WM_UNINITMENUPOPUP(%p, %u)", hMenu, nFlags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMenuCommand(HWND hwnd, UINT nPos, HMENU hMenu)
{
    char buf[512];
    wsprintfA(buf, "WM_MENUCOMMAND(%u, %p)", nPos, hMenu);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnChangeUIState(HWND hwnd, UINT nAction, UINT nUIElement)
{
    char buf[512];
    wsprintfA(buf, "WM_CHANGEUISTATE(%u, %u)", nAction, nUIElement);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnUpdateUIState(HWND hwnd, UINT nAction, UINT nUIElement)
{
    char buf[512];
    wsprintfA(buf, "WM_UPDATEUISTATE(%u, %u)", nAction, nUIElement);
    *md_hwnd2string(hwnd) = buf;
}

UINT md_OnQueryUIState(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_QUERYUISTATE()";
}

void md_OnXButtonDown(HWND hwnd, BOOL fDoubleClick, WORD fwKeys, WORD fwButton, INT xPos, INT yPos)
{
    char buf[512];
    if (fDoubleClick)
        wsprintfA(buf, "WM_XBUTTONDBLCLK(%d, %u, %u, %d, %d)", fDoubleClick, fwKeys, fwButton, xPos, yPos);
    else
        wsprintfA(buf, "WM_XBUTTONDOWN(%d, %u, %u, %d, %d)", fDoubleClick, fwKeys, fwButton, xPos, yPos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnXButtonUp(HWND hwnd, WORD fwKeys, WORD fwButton, INT xPos, INT yPos)
{
    char buf[512];
    wsprintfA(buf, "WM_XBUTTONUP(%u, %u, %d, %d)", fwKeys, fwButton, xPos, yPos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnEnterMenuLoop(HWND hwnd, BOOL bIsTrackPopupMenu)
{
    char buf[512];
    wsprintfA(buf, "WM_ENTERMENULOOP(%d)", bIsTrackPopupMenu);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnExitMenuLoop(HWND hwnd, BOOL bIsTrackPopupMenu)
{
    char buf[512];
    wsprintfA(buf, "WM_EXITMENULOOP(%d)", bIsTrackPopupMenu);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNextMenu(HWND hwnd, INT nCode, LPMDINEXTMENU lpMDINextMenu)
{
    char buf[512];
    wsprintfA(buf, "WM_NEXTMENU(%d, %p)", nCode, lpMDINextMenu);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnSizing(HWND hwnd, UINT nSide, LPRECT lpRect)
{
    char buf[512];
    wsprintfA(buf, "WM_SIZING(%u, %p)", nSide, lpRect);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnCaptureChanged(HWND hwnd, HWND hwndNewCapture)
{
    char buf[512];
    wsprintfA(buf, "WM_CAPTURECHANGED(%p)", hwndNewCapture);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMoving(HWND hwnd, UINT nSide, LPRECT lpRect)
{
    char buf[512];
    wsprintfA(buf, "WM_MOVING(%u, %p)", nSide, lpRect);
    *md_hwnd2string(hwnd) = buf;
}

LRESULT md_OnPowerBroadcast(HWND hwnd, UINT nPowerEvent, UINT nEventData)
{
    char buf[512];
    wsprintfA(buf, "WM_POWERBROADCAST(%u, %u)", nPowerEvent, nEventData);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnEnterSizeMove(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_ENTERSIZEMOVE()";
}

void md_OnExitSizeMove(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_EXITSIZEMOVE()";
}

HMENU md_MDIRefreshMenu(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_MDIREFRESHMENU()";
}

HMENU md_MDISetMenu(HWND hwnd, BOOL fRefresh, HMENU hmenuFrame, HMENU hmenuWindow)
{
    char buf[512];
    wsprintfA(buf, "WM_MDISETMENU(%d, %p, %p)", fRefresh, hmenuFrame, hmenuWindow);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnImeSetContext(HWND hwnd, BOOL fActive, DWORD dwShow)
{
    char buf[512];
    wsprintfA(buf, "WM_IME_SETCONTEXT(%d, 0x%08lX)", fActive, dwShow);
    *md_hwnd2string(hwnd) = buf;
}

LRESULT md_OnImeNotify(HWND hwnd, WPARAM wSubMessage, LPARAM lParam)
{
    char buf[512];
    wsprintfA(buf, "WM_IME_NOTIFY(%p, %p)", wSubMessage, lParam);
    *md_hwnd2string(hwnd) = buf;
}

LRESULT md_OnImeControl(HWND hwnd, WPARAM wSubMessage, LPVOID lpData)
{
    char buf[512];
    wsprintfA(buf, "WM_IME_CONTROL(%p, %p)", wSubMessage, lpData);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnImeCompositionFull(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_IME_COMPOSITIONFULL()";
}

void md_OnImeSelect(HWND hwnd, BOOL fSelect, HKL hKL)
{
    char buf[512];
    wsprintfA(buf, "WM_IME_SELECT(%d, %p)", fSelect, hKL);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnImeChar(HWND hwnd, WORD wCharCode, LONG lKeyData)
{
    char buf[512];
    wsprintfA(buf, "WM_IME_CHAR(%u, %ld)", wCharCode, lKeyData);
    *md_hwnd2string(hwnd) = buf;
}

LRESULT md_OnImeRequest(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    char buf[512];
    wsprintfA(buf, "WM_IME_REQUEST(%p, %p)", wParam, lParam);
    *md_hwnd2string(hwnd) = buf;
    return 0;
}

void md_OnImeKey(HWND hwnd, BOOL fDown, UINT nVirtKey, LONG lKeyData)
{
    char buf[512];
    if (fDown)
        wsprintfA(buf, "WM_IME_KEYDOWN(%d, %u, %ld)", fDown, nVirtKey, lKeyData);
    else
        wsprintfA(buf, "WM_IME_KEYUP(%d, %u, %ld)", fDown, nVirtKey, lKeyData);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMouseHover(HWND hwnd, UINT nFlags, INT xPos, INT yPos)
{
    char buf[512];
    wsprintfA(buf, "WM_MOUSEHOVER(%u, %d, %d)", nFlags, xPos, yPos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnMouseLeave(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_MOUSELEAVE()";
}

void md_OnNCMouseHover(HWND hwnd, UINT nHitTest, INT xPos, INT yPos)
{
    char buf[512];
    wsprintfA(buf, "WM_NCMOUSEHOVER(%u, %d, %d)", nHitTest, xPos, yPos);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnNCMouseLeave(HWND hwnd)
{
    *md_hwnd2string(hwnd) = "WM_NCMOUSELEAVE()";
}

void md_OnPrint(HWND hwnd, HDC hDC, UINT uFlags)
{
    char buf[512];
    wsprintfA(buf, "WM_PRINT(%p, %u)", hDC, uFlags);
    *md_hwnd2string(hwnd) = buf;
}

void md_OnPrintClient(HWND hwnd, HDC hDC, UINT uFlags)
{
    char buf[512];
    wsprintfA(buf, "WM_PRINTCLIENT(%p, %u)", hDC, uFlags);
    *md_hwnd2string(hwnd) = buf;
}

BOOL md_OnAppCommand(HWND hwnd, HWND hwndTarget, UINT cmd, UINT nDevice, UINT nKey)
{
    char buf[512];
    wsprintfA(buf, "WM_APPCOMMAND(%p, %u, %u, %u)", hwndTarget, cmd, nDevice, nKey);
    *md_hwnd2string(hwnd) = buf;
}

/****************************************************************************/
/* dumping */

LRESULT md_process(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_NULL, md_OnNull);
        HANDLE_MSG(hwnd, WM_CREATE, md_OnCreate);
        HANDLE_MSG(hwnd, WM_DESTROY, md_OnDestroy);
        HANDLE_MSG(hwnd, WM_MOVE, md_OnMove);
        HANDLE_MSG(hwnd, WM_SIZE, md_OnSize);
        HANDLE_MSG(hwnd, WM_ACTIVATE, md_OnActivate);
        HANDLE_MSG(hwnd, WM_SETFOCUS, md_OnSetFocus);
        HANDLE_MSG(hwnd, WM_KILLFOCUS, md_OnKillFocus);
        HANDLE_MSG(hwnd, WM_ENABLE, md_OnEnable);
        HANDLE_MSG(hwnd, WM_SETREDRAW, md_OnSetRedraw);
        HANDLE_MSG(hwnd, WM_SETTEXT, md_OnSetText);
        HANDLE_MSG(hwnd, WM_GETTEXT, md_OnGetText);
        HANDLE_MSG(hwnd, WM_GETTEXTLENGTH, md_OnGetTextLength);
        HANDLE_MSG(hwnd, WM_PAINT, md_OnPaint);
        HANDLE_MSG(hwnd, WM_CLOSE, md_OnClose);
#ifndef _WIN32_WCE
        HANDLE_MSG(hwnd, WM_QUERYENDSESSION, md_OnQueryEndSession);
        HANDLE_MSG(hwnd, WM_QUERYOPEN, md_OnQueryOpen);
        HANDLE_MSG(hwnd, WM_ENDSESSION, md_OnEndSession);
#endif
        HANDLE_MSG(hwnd, WM_QUIT, md_OnQuit);
        HANDLE_MSG(hwnd, WM_ERASEBKGND, md_OnEraseBkgnd);
        HANDLE_MSG(hwnd, WM_SYSCOLORCHANGE, md_OnSysColorChange);
        HANDLE_MSG(hwnd, WM_SHOWWINDOW, md_OnShowWindow);
        HANDLE_MSG(hwnd, WM_WININICHANGE, md_OnWinIniChange);
        //HANDLE_MSG(hwnd, WM_SETTINGCHANGE, md_OnSettingChange);   // duplicated to WM_WININICHANGE
        HANDLE_MSG(hwnd, WM_DEVMODECHANGE, md_OnDevModeChange);
        HANDLE_MSG(hwnd, WM_ACTIVATEAPP, md_OnActivateApp);
        HANDLE_MSG(hwnd, WM_FONTCHANGE, md_OnFontChange);
        HANDLE_MSG(hwnd, WM_TIMECHANGE, md_OnTimeChange);
        HANDLE_MSG(hwnd, WM_CANCELMODE, md_OnCancelMode);
        HANDLE_MSG(hwnd, WM_SETCURSOR, md_OnSetCursor);
        HANDLE_MSG(hwnd, WM_MOUSEACTIVATE, md_OnMouseActivate);
        HANDLE_MSG(hwnd, WM_CHILDACTIVATE, md_OnChildActivate);
        HANDLE_MSG(hwnd, WM_QUEUESYNC, md_OnQueueSync);
        HANDLE_MSG(hwnd, WM_GETMINMAXINFO, md_OnGetMinMaxInfo);
        HANDLE_MSG(hwnd, WM_PAINTICON, md_OnPaintIcon);
        HANDLE_MSG(hwnd, WM_ICONERASEBKGND, md_OnIconEraseBkgnd);
        HANDLE_MSG(hwnd, WM_NEXTDLGCTL, md_OnNextDlgCtl);
        HANDLE_MSG(hwnd, WM_SPOOLERSTATUS, md_OnSpoolerStatus);
        HANDLE_MSG(hwnd, WM_DRAWITEM, md_OnDrawItem);
        HANDLE_MSG(hwnd, WM_MEASUREITEM, md_OnMeasureItem);
        HANDLE_MSG(hwnd, WM_DELETEITEM, md_OnDeleteItem);
        HANDLE_MSG(hwnd, WM_VKEYTOITEM, md_OnVkeyToItem);
        HANDLE_MSG(hwnd, WM_CHARTOITEM, md_OnCharToItem);
        HANDLE_MSG(hwnd, WM_SETFONT, md_OnSetFont);
        HANDLE_MSG(hwnd, WM_GETFONT, md_OnGetFont);
        HANDLE_MSG(hwnd, WM_SETHOTKEY, md_OnSetHotKey);
        HANDLE_MSG(hwnd, WM_GETHOTKEY, md_OnGetHotKey);
        HANDLE_MSG(hwnd, WM_QUERYDRAGICON, md_OnQueryDragIcon);
        HANDLE_MSG(hwnd, WM_COMPAREITEM, md_OnCompareItem);
#if WINVER >= 0x0500
# ifndef _WIN32_WCE
        HANDLE_MSG(hwnd, WM_GETOBJECT, md_OnGetObject);
# endif
#endif
        HANDLE_MSG(hwnd, WM_COMPACTING, md_OnCompacting);
        HANDLE_MSG(hwnd, WM_COMMNOTIFY, md_OnCommNotify);
        HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGING, md_OnWindowPosChanging);
        HANDLE_MSG(hwnd, WM_WINDOWPOSCHANGED, md_OnWindowPosChanged);
        HANDLE_MSG(hwnd, WM_POWER, md_OnPower);
        HANDLE_MSG(hwnd, WM_COPYDATA, md_OnCopyData);
        HANDLE_MSG(hwnd, WM_CANCELJOURNAL, md_OnCancelJournal);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_NOTIFY, md_OnNotify);
        HANDLE_MSG(hwnd, WM_INPUTLANGCHANGEREQUEST, md_OnInputLangChangeRequest);
        HANDLE_MSG(hwnd, WM_INPUTLANGCHANGE, md_OnInputLangChange);
        HANDLE_MSG(hwnd, WM_TCARD, md_OnTCard);
        HANDLE_MSG(hwnd, WM_HELP, md_OnHelp);
        HANDLE_MSG(hwnd, WM_USERCHANGED, md_OnUserChanged);
        HANDLE_MSG(hwnd, WM_NOTIFYFORMAT, md_OnNotifyFormat);
        HANDLE_MSG(hwnd, WM_CONTEXTMENU, md_OnContextMenu);
        HANDLE_MSG(hwnd, WM_STYLECHANGING, md_OnStyleChanging);
        HANDLE_MSG(hwnd, WM_STYLECHANGED, md_OnStyleChanged);
        HANDLE_MSG(hwnd, WM_DISPLAYCHANGE, md_OnDisplayChange);
        HANDLE_MSG(hwnd, WM_GETICON, md_OnGetIcon);
        HANDLE_MSG(hwnd, WM_SETICON, md_OnSetIcon);
#endif
        HANDLE_MSG(hwnd, WM_NCCREATE, md_OnNCCreate);
        HANDLE_MSG(hwnd, WM_NCDESTROY, md_OnNCDestroy);
        HANDLE_MSG(hwnd, WM_NCCALCSIZE, md_OnNCCalcSize);
        HANDLE_MSG(hwnd, WM_NCHITTEST, md_OnNCHitTest);
        HANDLE_MSG(hwnd, WM_NCPAINT, md_OnNCPaint);
        HANDLE_MSG(hwnd, WM_NCACTIVATE, md_OnNCActivate);
        HANDLE_MSG(hwnd, WM_GETDLGCODE, md_OnGetDlgCode);
#ifndef _WIN32_WCE
        HANDLE_MSG(hwnd, WM_SYNCPAINT, md_OnSyncPaint);
#endif
        HANDLE_MSG(hwnd, WM_NCMOUSEMOVE, md_OnNCMouseMove);
        HANDLE_MSG(hwnd, WM_NCLBUTTONDOWN, md_OnNCLButtonDown);
        HANDLE_MSG(hwnd, WM_NCLBUTTONUP, md_OnNCLButtonUp);
        HANDLE_MSG(hwnd, WM_NCLBUTTONDBLCLK, md_OnNCLButtonDown);
        HANDLE_MSG(hwnd, WM_NCRBUTTONDOWN, md_OnNCRButtonDown);
        HANDLE_MSG(hwnd, WM_NCRBUTTONUP, md_OnNCRButtonUp);
        HANDLE_MSG(hwnd, WM_NCRBUTTONDBLCLK, md_OnNCRButtonDown);
        HANDLE_MSG(hwnd, WM_NCMBUTTONDOWN, md_OnNCMButtonDown);
        HANDLE_MSG(hwnd, WM_NCMBUTTONUP, md_OnNCMButtonUp);
        HANDLE_MSG(hwnd, WM_NCMBUTTONDBLCLK, md_OnNCMButtonDown);
#if _WIN32_WINNT >= 0x0500
        HANDLE_MSG(hwnd, WM_NCXBUTTONDOWN, md_OnNCXButtonDown);
        HANDLE_MSG(hwnd, WM_NCXBUTTONUP, md_OnNCXButtonUp);
        HANDLE_MSG(hwnd, WM_NCXBUTTONDBLCLK, md_OnNCXButtonDown);
#endif
        HANDLE_MSG(hwnd, WM_KEYDOWN, md_OnKey);
        HANDLE_MSG(hwnd, WM_KEYUP, md_OnKey);
        HANDLE_MSG(hwnd, WM_CHAR, md_OnChar);
        HANDLE_MSG(hwnd, WM_DEADCHAR, md_OnDeadChar);
        HANDLE_MSG(hwnd, WM_SYSKEYDOWN, md_OnSysKey);
        HANDLE_MSG(hwnd, WM_SYSKEYUP, md_OnSysKey);
        HANDLE_MSG(hwnd, WM_SYSCHAR, md_OnSysChar);
        HANDLE_MSG(hwnd, WM_SYSDEADCHAR, md_OnSysDeadChar);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_IME_STARTCOMPOSITION, md_OnImeStartComposition);
        HANDLE_MSG(hwnd, WM_IME_ENDCOMPOSITION, md_OnImeEndComposition);
        HANDLE_MSG(hwnd, WM_IME_COMPOSITION, md_OnImeComposition);
#endif
        HANDLE_MSG(hwnd, WM_INITDIALOG, md_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, md_OnCommand);
        HANDLE_MSG(hwnd, WM_SYSCOMMAND, md_OnSysCommand);
        HANDLE_MSG(hwnd, WM_TIMER, md_OnTimer);
        HANDLE_MSG(hwnd, WM_HSCROLL, md_OnHScroll);
        HANDLE_MSG(hwnd, WM_VSCROLL, md_OnVScroll);
        HANDLE_MSG(hwnd, WM_INITMENU, md_OnInitMenu);
        HANDLE_MSG(hwnd, WM_INITMENUPOPUP, md_OnInitMenuPopup);
        HANDLE_MSG(hwnd, WM_MENUSELECT, md_OnMenuSelect);
        HANDLE_MSG(hwnd, WM_MENUCHAR, md_OnMenuChar);
        HANDLE_MSG(hwnd, WM_ENTERIDLE, md_OnEnterIdle);
#if WINVER >= 0x0500
# ifndef _WIN32_WCE
        HANDLE_MSG(hwnd, WM_MENURBUTTONUP, md_OnMenuRButtonUp);
        HANDLE_MSG(hwnd, WM_MENUDRAG, md_OnMenuDrag);
        HANDLE_MSG(hwnd, WM_MENUGETOBJECT, md_OnMenuGetObject);
        HANDLE_MSG(hwnd, WM_UNINITMENUPOPUP, md_OnUninitMenuPopup);
        HANDLE_MSG(hwnd, WM_MENUCOMMAND, md_OnMenuCommand);
#  ifndef _WIN32_WCE
#   if _WIN32_WINNT >= 0x0500
        HANDLE_MSG(hwnd, WM_CHANGEUISTATE, md_OnChangeUIState);
        HANDLE_MSG(hwnd, WM_UPDATEUISTATE, md_OnUpdateUIState);
        HANDLE_MSG(hwnd, WM_QUERYUISTATE, md_OnQueryUIState);
#   endif
#  endif
# endif
#endif
        HANDLE_MSG(hwnd, WM_CTLCOLORMSGBOX, md_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLOREDIT, md_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORLISTBOX, md_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORBTN, md_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORDLG, md_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORSCROLLBAR, md_OnCtlColor);
        HANDLE_MSG(hwnd, WM_CTLCOLORSTATIC, md_OnCtlColor);
        HANDLE_MSG(hwnd, WM_MOUSEMOVE, md_OnMouseMove);
        HANDLE_MSG(hwnd, WM_LBUTTONDOWN, md_OnLButtonDown);
        HANDLE_MSG(hwnd, WM_LBUTTONUP, md_OnLButtonUp);
        HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, md_OnLButtonDown);
        HANDLE_MSG(hwnd, WM_RBUTTONDOWN, md_OnRButtonDown);
        HANDLE_MSG(hwnd, WM_RBUTTONUP, md_OnRButtonUp);
        HANDLE_MSG(hwnd, WM_RBUTTONDBLCLK, md_OnRButtonDown);
        HANDLE_MSG(hwnd, WM_MBUTTONDOWN, md_OnMButtonDown);
        HANDLE_MSG(hwnd, WM_MBUTTONUP, md_OnMButtonUp);
        HANDLE_MSG(hwnd, WM_MBUTTONDBLCLK, md_OnMButtonDown);
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
        HANDLE_MSG(hwnd, WM_MOUSEWHEEL, md_OnMouseWheel);
#endif
#if _WIN32_WINNT >= 0x0500
        HANDLE_MSG(hwnd, WM_XBUTTONDOWN, md_OnXButtonDown);
        HANDLE_MSG(hwnd, WM_XBUTTONUP, md_OnXButtonUp);
        HANDLE_MSG(hwnd, WM_XBUTTONDBLCLK, md_OnXButtonDown);
#endif
        HANDLE_MSG(hwnd, WM_PARENTNOTIFY, md_OnParentNotify);
        HANDLE_MSG(hwnd, WM_ENTERMENULOOP, md_OnEnterMenuLoop);
        HANDLE_MSG(hwnd, WM_EXITMENULOOP, md_OnExitMenuLoop);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_NEXTMENU, md_OnNextMenu);
        HANDLE_MSG(hwnd, WM_SIZING, md_OnSizing);
        HANDLE_MSG(hwnd, WM_CAPTURECHANGED, md_OnCaptureChanged);
        HANDLE_MSG(hwnd, WM_MOVING, md_OnMoving);
        HANDLE_MSG(hwnd, WM_POWERBROADCAST, md_OnPowerBroadcast);
        HANDLE_MSG(hwnd, WM_DEVICECHANGE, md_OnDeviceChange);
#endif
        HANDLE_MSG(hwnd, WM_MDICREATE, md_MDICreate);
        HANDLE_MSG(hwnd, WM_MDIDESTROY, md_MDIDestroy);
        HANDLE_MSG(hwnd, WM_MDIACTIVATE, md_MDIActivate);
        HANDLE_MSG(hwnd, WM_MDIRESTORE, md_MDIRestore);
        HANDLE_MSG(hwnd, WM_MDINEXT, md_MDINext);
        HANDLE_MSG(hwnd, WM_MDIMAXIMIZE, md_MDIMaximize);
        HANDLE_MSG(hwnd, WM_MDITILE, md_MDITile);
        HANDLE_MSG(hwnd, WM_MDICASCADE, md_MDICascade);
        HANDLE_MSG(hwnd, WM_MDIICONARRANGE, md_MDIIconArrange);
        HANDLE_MSG(hwnd, WM_MDIGETACTIVE, md_MDIGetActive);
        HANDLE_MSG(hwnd, WM_MDISETMENU, md_MDISetMenu);
        HANDLE_MSG(hwnd, WM_ENTERSIZEMOVE, md_OnEnterSizeMove);
        HANDLE_MSG(hwnd, WM_EXITSIZEMOVE, md_OnExitSizeMove);
        HANDLE_MSG(hwnd, WM_DROPFILES, md_OnDropFiles);
        HANDLE_MSG(hwnd, WM_MDIREFRESHMENU, md_MDIRefreshMenu);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_IME_SETCONTEXT, md_OnImeSetContext);
        HANDLE_MSG(hwnd, WM_IME_NOTIFY, md_OnImeNotify);
        HANDLE_MSG(hwnd, WM_IME_CONTROL, md_OnImeControl);
        HANDLE_MSG(hwnd, WM_IME_COMPOSITIONFULL, md_OnImeCompositionFull);
        HANDLE_MSG(hwnd, WM_IME_SELECT, md_OnImeSelect);
        HANDLE_MSG(hwnd, WM_IME_CHAR, md_OnImeChar);
#endif
#if WINVER >= 0x0500
        HANDLE_MSG(hwnd, WM_IME_REQUEST, md_OnImeRequest);
#endif
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_IME_KEYDOWN, md_OnImeKey);
        HANDLE_MSG(hwnd, WM_IME_KEYUP, md_OnImeKey);
#endif
#if (_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500)
        HANDLE_MSG(hwnd, WM_MOUSEHOVER, md_OnMouseHover);
        HANDLE_MSG(hwnd, WM_MOUSELEAVE, md_OnMouseLeave);
#endif
#if WINVER >= 0x0500
        HANDLE_MSG(hwnd, WM_NCMOUSEHOVER, md_OnNCMouseHover);
        HANDLE_MSG(hwnd, WM_NCMOUSELEAVE, md_OnNCMouseLeave);
#endif
        HANDLE_MSG(hwnd, WM_CUT, md_OnCut);
        HANDLE_MSG(hwnd, WM_COPY, md_OnCopy);
        HANDLE_MSG(hwnd, WM_PASTE, md_OnPaste);
        HANDLE_MSG(hwnd, WM_CLEAR, md_OnClear);
        HANDLE_MSG(hwnd, WM_UNDO, md_OnUndo);
        HANDLE_MSG(hwnd, WM_RENDERFORMAT, md_OnRenderFormat);
        HANDLE_MSG(hwnd, WM_RENDERALLFORMATS, md_OnRenderAllFormats);
        HANDLE_MSG(hwnd, WM_DESTROYCLIPBOARD, md_OnDestroyClipboard);
        HANDLE_MSG(hwnd, WM_DRAWCLIPBOARD, md_OnDrawClipboard);
        HANDLE_MSG(hwnd, WM_PAINTCLIPBOARD, md_OnPaintClipboard);
        HANDLE_MSG(hwnd, WM_VSCROLLCLIPBOARD, md_OnVScrollClipboard);
        HANDLE_MSG(hwnd, WM_SIZECLIPBOARD, md_OnSizeClipboard);
        HANDLE_MSG(hwnd, WM_ASKCBFORMATNAME, md_OnAskCBFormatName);
        HANDLE_MSG(hwnd, WM_CHANGECBCHAIN, md_OnChangeCBChain);
        HANDLE_MSG(hwnd, WM_HSCROLLCLIPBOARD, md_OnHScrollClipboard);
        HANDLE_MSG(hwnd, WM_QUERYNEWPALETTE, md_OnQueryNewPalette);
        HANDLE_MSG(hwnd, WM_PALETTEISCHANGING, md_OnPaletteIsChanging);
        HANDLE_MSG(hwnd, WM_PALETTECHANGED, md_OnPaletteChanged);
        HANDLE_MSG(hwnd, WM_HOTKEY, md_OnHotKey);
#if WINVER >= 0x0400
        HANDLE_MSG(hwnd, WM_PRINT, md_OnPrint);
        HANDLE_MSG(hwnd, WM_PRINTCLIENT, md_OnPrintClient);
#endif
#if _WIN32_WINNT >= 0x0500
        HANDLE_MSG(hwnd, WM_APPCOMMAND, md_OnAppCommand);
#endif
    default:
        if (WM_USER <= uMsg && uMsg <= 0x7FFF)
        {
            return md_OnUser(hwnd, uMsg, wParam, lParam);
        }
        if (WM_APP <= uMsg && uMsg <= 0xBFFF)
        {
            return md_OnApp(hwnd, uMsg, wParam, lParam);
        }
        return md_OnUnknown(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

std::string md_dump(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    std::string str;
    HWND hwnd = reinterpret_cast<HWND>(&str);
    md_process(hwnd, uMsg, wParam, lParam);
    return str;
}

/****************************************************************************/
