// Author: yangqi
/**
 * \file PopupWindow.h
 * \brief 
 * \author yangqi
 * \date October 2017
 *
 */
#pragma once

class CGifImage;

class CPopupWindow : public CWindowImpl<CPopupWindow>
{
public:
    //DECLARE_WND_CLASS(L"")
    CPopupWindow();
    ~CPopupWindow();

    BEGIN_MSG_MAP(CPopupWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
        MESSAGE_HANDLER(WM_TIMER, OnTimer)
        MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDbClick)
        MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
    END_MSG_MAP()

    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnLButtonDbClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);


private:
    bool Load(LPCWSTR lpszName, bool bIsFile);
    bool GetFileOpen(WCHAR *pszFileName, DWORD cchFileName);
    void CalcWindowRect();
    void OnDraw(HDC hDC);

private:
    const UINT m_nTimerID = 1111;
    CGifImage* m_pImage = nullptr;
};

