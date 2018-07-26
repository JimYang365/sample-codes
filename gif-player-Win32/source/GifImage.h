// Author: yangqi
/**
 * \file GifImage.h
 * \brief 
 * \author yangqi
 * \date October 2017
 *
 */
#pragma once
class CGifImage : public Gdiplus::Image
{
public:
    CGifImage(LPCWSTR lpszFileName, int nPauseMultiple = 1);
    CGifImage(LPBYTE pBuf, size_t nSize, int nPauseMultiple = 1);

    ~CGifImage();

    void Draw(HDC hDC, int x, int y, UINT cx, UINT cy);
    UINT NextFrame();

private:
    bool TestForAnimatedGIF();
    bool LoadFromBuffer(BYTE* pBuff, int nSize);

private:
    const int m_nPauseMultiple;
    UINT m_nFrameCount = 0;
    Gdiplus::PropertyItem* m_pPropertyItem = nullptr;
    UINT m_nFramePosition = 0;
};

