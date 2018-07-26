// GifPlayer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DPIUtils.h"
#include "GifPlayer.h"
#include "PopupWindow.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    GdiplusStartupInput		gdiplusStartupInput;
    ULONG_PTR				gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CPopupWindow wndMain;
    RECT rc = { 500, 500, 100, 100 };
    if (wndMain.Create(nullptr, rc, nullptr, WS_POPUP, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST) == NULL)
    {
        return 0;
    }

   // ::SetLayeredWindowAttributes(wndMain, 0, 255, LWA_ALPHA);
    wndMain.ShowWindow(nCmdShow);

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    se::EnableDPIAware();

    HRESULT hRes = ::CoInitialize(NULL);
    // If you are running on NT 4.0 or higher you can use the following call instead to 
    // make the EXE free threaded. This means that calls come in on a random RPC thread.
    //	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ATLASSERT(SUCCEEDED(hRes));

    // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    ::CoUninitialize();

    return nRet;
}
