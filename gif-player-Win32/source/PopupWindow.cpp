#include "stdafx.h"
#include "PopupWindow.h"
#include "GifImage.h"
#include "resource.h"
#include <commdlg.h>


CPopupWindow::CPopupWindow()
{
}


CPopupWindow::~CPopupWindow()
{
    delete m_pImage;
}


LRESULT CPopupWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    Load(MAKEINTRESOURCE(IDR_GIF_DEMO), false);
    ShowWindow(SW_SHOW);

    return 0;
}

LRESULT CPopupWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    ::PostQuitMessage(1);
    return 1;
}

LRESULT CPopupWindow::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 0;
}

static void DrawDemoText(HDC hdc)
{
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::Font myFont(L"ËÎÌו", 16);
    Gdiplus::PointF pt;
    Gdiplus::SolidBrush   textBrush(Gdiplus::Color(200, 255, 0, 0));
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    graphics.DrawString(L"This is a DEMO\r\nLButton Click to select file.\r\nRButton Clitk to quit.", -1, &myFont, pt, &textBrush);
}

void CPopupWindow::OnDraw(HDC hDC)
{
    CRect rc = { 0 };
    GetWindowRect(&rc);
    const auto nWidth = rc.Width();
    const auto nHeight = rc.Height();

    //
    HDC hdcMem = ::CreateCompatibleDC(hDC);
    HBITMAP hBitMap = ::CreateCompatibleBitmap(hDC, rc.Width(), rc.Height());
    HBITMAP hOldBmp = (HBITMAP)::SelectObject(hdcMem, hBitMap);

    //
    m_pImage->Draw(hdcMem, 0, 0, nWidth, nHeight);

    //
    DrawDemoText(hdcMem);

    //
    SIZE sz = { nWidth, nHeight };
    POINT ptSrc = { 0, 0 };
    POINT ptDest = { rc.left, rc.top };
    BLENDFUNCTION stBlend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    ::UpdateLayeredWindow(m_hWnd, hDC, &ptDest, &sz, hdcMem, &ptSrc, 0, &stBlend, ULW_ALPHA);

    //
    ::SelectObject(hdcMem, hOldBmp);
    ::DeleteDC(hdcMem);
    ::DeleteObject(hBitMap);
}

LRESULT CPopupWindow::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(&ps);

    // do nothing

    ReleaseDC(hDC);
    EndPaint(&ps);

    return 0;
}

LRESULT CPopupWindow::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if (wParam != m_nTimerID)
    {
        bHandled = FALSE;
        return 0;
    }

    KillTimer(wParam);

    SetTimer(m_nTimerID, m_pImage->NextFrame());

    HDC hDC = GetDC();
    OnDraw(hDC);
    ReleaseDC(hDC);

    return 0;
}

LRESULT CPopupWindow::OnLButtonDbClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    WCHAR szFileName[MAX_PATH] = { 0 };

    if (GetFileOpen(szFileName, _countof(szFileName)))
    {
        Load(szFileName, true);
    }

    return 0;
}

LRESULT CPopupWindow::OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    PostMessage(WM_CLOSE);
    return 0;
}


bool CPopupWindow::Load(LPCWSTR lpszName, bool bIsFile)
{
    KillTimer(m_nTimerID);
    if (m_pImage)
    {
        delete m_pImage;
    }

    if (bIsFile)
    {
        m_pImage = new CGifImage(lpszName);
    }
    else // load from resource
    {
        HRSRC hSrc = ::FindResource(ModuleHelper::GetResourceInstance(), lpszName, L"GIF");
        DWORD dwResLen = ::SizeofResource(ModuleHelper::GetResourceInstance(), hSrc);
        HGLOBAL hGlobal = ::LoadResource(ModuleHelper::GetResourceInstance(), hSrc);

        BYTE * pBuff = new BYTE[dwResLen];
        memcpy(pBuff, ::LockResource(hGlobal), dwResLen);

        m_pImage = new CGifImage(pBuff, dwResLen);

        delete[] pBuff;
        pBuff = nullptr;
    }

    //
    CalcWindowRect();

    // play animation
    SetTimer(m_nTimerID, 0);

    return true;

}

bool CPopupWindow::GetFileOpen(WCHAR *pszFileName, DWORD cchFileName)
{
    pszFileName[0] = L'\0';

    OPENFILENAME ofn;
    RtlZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = L"*Gif Files\0*.gif\0";
    ofn.lpstrFile = pszFileName;
    ofn.nMaxFile = cchFileName;
    ofn.lpstrTitle = L"Select an image to display...";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    // Display the Open dialog box.
    return (GetOpenFileName(&ofn) == TRUE);
}

void CPopupWindow::CalcWindowRect()
{
    RECT rc = { 0 };
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

    rc.left = rc.right - m_pImage->GetWidth();
    rc.top = rc.bottom - m_pImage->GetHeight();

    MoveWindow(&rc);
}
